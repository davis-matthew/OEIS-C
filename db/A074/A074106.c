/*
 * A074106  a(1)=1; a(n) is the minimal unused odd prime such that the sum
 *          a(1)+a(2)+...+a(n) is divisible by n.
 * https://oeis.org/A074106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   gcd, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A074106: a(1)=1; for n≥2, a(n) is the minimal unused odd prime so that
   a(1)+…+a(n) ≡ 0 (mod n).  Sequence stops when no such prime exists. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
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

static seq_t *a_cache      = NULL;   /* stored terms a(1)…a(k)          */
static size_t a_cap        = 0;       /* capacity of a_cache             */
static size_t a_len        = 0;       /* number of stored terms           */
static seq_t cumulative_sum = 0;      /* sum of stored terms              */
static bool terminated     = false;  /* true once the sequence stops     */

static seq_t *prime_list   = NULL;   /* list of odd primes found so far   */
static size_t prime_cap    = 0;       /* capacity of prime_list           */
static size_t prime_cnt    = 0;       /* number of primes in prime_list   */
static seq_t next_candidate = 3;     /* next odd candidate to test       */

/* Ensure the a_cache array can hold at least 'need' elements. */
static void ensure_a_capacity(size_t need)
{
    if (need <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, a_cache, new_cap);
    if (!tmp) abort();
    a_cache = tmp;
    a_cap   = new_cap;
}

/* Ensure the prime_list array can hold at least 'need' primes. */
static void ensure_prime_capacity(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, prime_list, new_cap);
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Generate the next odd prime, append it to prime_list, and return it. */
static seq_t generate_next_prime(void)
{
    while (true) {
        if (is_prime_big(next_candidate)) {
            ensure_prime_capacity(prime_cnt + 1);
            prime_list[prime_cnt++] = next_candidate;
            seq_t p = next_candidate;
            next_candidate += 2;  /* skip even numbers */
            return p;
        }
        next_candidate += 2;
    }
}

/* Test whether 'x' is already present in the sequence cache. */
static bool used_in_cache(seq_t x)
{
    for (size_t i = a_len; i--;)
        if (a_cache[i] == x) return true;
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* If the sequence already terminated and n is beyond cached terms,
       return 0. */
    if (terminated && n > a_len) return 0;

    /* Initialize with the first term if necessary. */
    if (a_len == 0) {
        ensure_a_capacity(1);
        a_cache[0] = 1;
        a_len = 1;
        cumulative_sum = 1;
    }

    /* Compute terms up to index 'n'. */
    for (idx_t idx = a_len + 1; idx <= n; ++idx) {
        seq_t i = (seq_t)idx;

        /* Residue required for the next prime so that sum ≡ 0 (mod i). */
        seq_t mod_sum = cumulative_sum % i;
        seq_t required = (i - mod_sum) % i;

        /* Quick check: if gcd(i, required) > 1 and required ≠ gcd,
           no prime can satisfy the congruence.  Stop the sequence. */
        seq_t g = gcd(i, required);
        if (g > (seq_t)1 && required != g) {
            /* Fill remaining requested positions with 0. */
            for (size_t j = a_len; j < (size_t)n; ++j) {
                ensure_a_capacity(j + 1);
                a_cache[j] = 0;
            }
            a_len = (size_t)n;
            terminated = true;
            return 0;
        }

        /* Search for the minimal unused odd prime with the required residue. */
        size_t prime_idx = 0;
        while (true) {
            /* Extend the prime list if needed. */
            if (prime_idx == prime_cnt) generate_next_prime();

            seq_t p = prime_list[prime_idx++];
            if (used_in_cache(p)) continue;
            if ((p % i) == required) {
                ensure_a_capacity(a_len + 1);
                a_cache[a_len] = p;
                cumulative_sum += p;
                ++a_len;
                break;
            }
        }
    }

    return a_cache[n - 1];
}
