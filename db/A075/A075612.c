/*
 * A075612  a(1) = 1, a(n) = smallest prime number not already used such that
 *          concatenation of a(k) and a(n) is composite for all k = 1 to n-1.
 * https://oeis.org/A075612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest unused prime p such that for all k < n, the
 *            concatenation of a(k) and p is composite. The implementation
 *            builds the sequence incrementally, memoizing computed terms and
 *            the set of primes already used. Concatenation of
 *            arbitrary‑precision terms is performed by string conversion,
 *            so...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <stddef.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

static seq_t *seq_cache = NULL;          /* 1‑based sequence terms (stored at seq_cache[0]) */
static size_t seq_cap  = 0;              /* current allocated capacity for seq_cache */
static size_t seq_len  = 0;              /* number of terms stored so far */

static uint64_t *used_primes = NULL;     /* list of primes already selected */
static size_t used_cap  = 0;
static size_t used_len  = 0;

static uint64_t last_prime = 1;          /* last prime examined */

/* Ensure seq_cache has space for at least n elements. */
static void ensure_seq_cap(size_t n)
{
    if (n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < n) new_cap *= 2;
    seq_cache = mem_resize(seq_t, seq_cache, new_cap);
    seq_cap = new_cap;
}

/* Ensure used_primes has space for at least n elements. */
static void ensure_used_cap(size_t n)
{
    if (n <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 64;
    while (new_cap < n) new_cap *= 2;
    used_primes = (uint64_t *)realloc(used_primes, new_cap * sizeof(uint64_t));
    used_cap = new_cap;
}

/* Test whether p is already in used_primes. */
static bool prime_used(uint64_t p)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used_primes[i] == p) return true;
    return false;
}

/* Add p to used_primes list. */
static void add_used_prime(uint64_t p)
{
    ensure_used_cap(used_len + 1);
    used_primes[used_len++] = p;
}

/* Concatenate a(seq_t) and b(uint64_t) into a seq_t. */
static seq_t concat_terms(seq_t a_term, uint64_t b)
{
    /* Buffer sizes: previous term and candidate are unlikely to exceed 256 digits. */
    char buf_a[256], buf_b[32], buf_concat[512];
    size_t len_a, len_b;

    seq_to_str(a_term, buf_a, &len_a);
    buf_a[len_a] = '\0';

    snprintf(buf_b, sizeof buf_b, "%llu", (unsigned long long)b);
    len_b = strlen(buf_b);

    memcpy(buf_concat, buf_a, len_a);
    memcpy(buf_concat + len_a, buf_b, len_b);
    buf_concat[len_a + len_b] = '\0';

    return seq_from_str(buf_concat);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for 0 but harmless */

    ensure_seq_cap((size_t)n);

    /* Compute missing terms up to n. */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        if (i == 1) {          /* a(1) = 1 */
            seq_cache[0] = (seq_t)1;
            /* 1 is not a prime, but it's the fixed starting value. */
        } else {
            uint64_t candidate = next_prime(last_prime);
            for (;;) {
                if (!prime_used(candidate)) {
                    bool ok = true;
                    for (size_t k = 0; k < (size_t)(i - 1); ++k) {
                        seq_t conc = concat_terms(seq_cache[k], candidate);
                        if (is_prime_big(conc)) { ok = false; break; }
                    }
                    if (ok) {
                        seq_cache[i - 1] = (seq_t)candidate;
                        add_used_prime(candidate);
                        last_prime = candidate;
                        break;
                    }
                }
                candidate = next_prime(candidate);
            }
        }
    }

    seq_len = (size_t)n;
    return seq_cache[n - 1];
}
