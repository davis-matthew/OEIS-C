/*
 * A174905  Numbers with no pair (d,e) of divisors such that d < e < 2*d.
 * https://oeis.org/A174905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer with no pair (d,e) of divisors such that
 *            d < e < 2*d. Computed by incremental search
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define MAX_CACHE 50000                /* enough for n up to ~10 000 */

static uint64_t cache[MAX_CACHE];
static size_t    cache_len   = 0;
static uint64_t  next_candidate = 1;

/* comparison function used by qsort to sort divisors */
static int cmp_uint64(const void *a, const void *b) {
    uint64_t v1 = *(const uint64_t *)a;
    uint64_t v2 = *(const uint64_t *)b;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* test whether a 64‑bit integer n has the required divisor property */
static bool is_good(uint64_t n) {
    if (n <= 1) return true;          /* divisors: {1} */

    uint64_t tmp = n;
    uint64_t primes[16];
    int       exps[16];
    int       pcnt = 0;

    /* factor n by trial division */
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]  = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {                    /* remaining prime factor */
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* generate all divisors */
    uint64_t divisors[256];
    size_t   dcnt = 1;
    divisors[0] = 1;

    for (int i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        int       e = exps[i];
        size_t old = dcnt;
        uint64_t pow = 1;
        for (int k = 1; k <= e; ++k) {
            pow *= p;
            for (size_t j = 0; j < old; ++j) {
                divisors[dcnt++] = divisors[j] * pow;
            }
        }
    }

    /* sort the divisors */
    qsort(divisors, dcnt, sizeof(uint64_t), cmp_uint64);

    /* check the divisor property */
    for (size_t i = 0; i + 1 < dcnt; ++i) {
        if (divisors[i + 1] < 2 * divisors[i]) return false;
    }
    return true;
}

/* sequence function required by the spec */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n=0, not used */

    while (cache_len < (size_t)n) {
        if (is_good(next_candidate)) {
            /* store the value as a 64‑bit integer; conversion to seq_t
               happens on return.  The cache is only used for the
               64‑bit representation, avoiding dynamic allocation of seq_t. */
            if (cache_len < MAX_CACHE) {
                cache[cache_len++] = next_candidate;
            }
        }
        ++next_candidate;
    }
    return (seq_t)cache[n - 1];
}
