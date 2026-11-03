/*
 * A140776  A number n is included if (p + n/p) is prime, where p is the
 *          largest prime that divides n.
 * https://oeis.org/A140776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 9256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9510 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A140776: n is included iff p + n/p is prime,
 * where p is the largest prime divisor of n.
 * The function returns the nth term (1‑based index).
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Return the largest prime divisor of m (m >= 2). */
static uint64_t largest_prime_divisor(uint64_t m)
{
    uint64_t largest = 0;
    /* factor 2 */
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    /* odd factors */
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)            /* remaining m is prime */
        largest = m;
    return largest;
}

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

static seq_t *terms = NULL;   /* cached sequence values */
static size_t terms_cap = 0;  /* capacity of terms array */
static size_t terms_cnt = 0;  /* number of stored terms */
static idx_t current = 2;     /* next integer to test */

/* Append a value to the static cache, growing the storage if necessary. */
static void append_term(seq_t v)
{
    if (terms_cnt == terms_cap) {
        terms_cap = terms_cap ? terms_cap << 1 : 16;
        terms = mem_resize(seq_t, terms, terms_cap);
    }
    terms[terms_cnt++] = v;
}

/* Main sequence function. Generates terms lazily until the requested
   index is available.  Caches results for subsequent calls in any order. */
seq_t a(idx_t n)
{
    if (!terms) {
        /* initialise cache */
        terms_cap = 16;
        terms = mem_new(seq_t, terms_cap);
        terms_cnt = 0;
        current = 2;
    }

    /* produce terms until we have at least n terms */
    while (terms_cnt < n) {
        uint64_t p = largest_prime_divisor((uint64_t)current);
        seq_t val = (seq_t)p + (seq_t)(current / p);
        if (is_prime_big(val)) {
            append_term((seq_t)current);
        }
        current++;
    }

    return terms[n - 1];
}
