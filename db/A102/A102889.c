/*
 * A102889  Square of the minimal space diagonal of an integral cuboid having
 *          volume n.
 * https://oeis.org/A102889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   int_sqrt, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal-space-diagonal squared for a volume n:
   a(n) = min{ a² + b² + c² | a·b·c = n, 1 ≤ a ≤ b ≤ c } */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"   /* returns idx_t */
#include "../utils/simple_sieve.h"  /* simple_sieve */

/* Comparison function for qsort on idx_t values */
static int cmp_idx(const void *p, const void *q)
{
    idx_t a = *(const idx_t *)p;
    idx_t b = *(const idx_t *)q;
    return (a < b) ? -1 : (a > b);
}

/* Global data for prime caching */
static uint32_t *primes = NULL;
static size_t    primes_cnt = 0;
static uint32_t  primes_lim = 0;

/* Generate primes ≤ limit and store in global array */
static void ensure_primes(uint32_t limit)
{
    if (limit <= primes_lim)
        return;
    if (primes)
        free(primes);
    simple_sieve(limit, &primes, &primes_cnt);
    primes_lim = limit;
}

/* Compute all divisors of n; returns array and count via pointers.
   Caller must free the returned array. */
static size_t compute_divisors(idx_t n, idx_t **divs)
{
    /* Factorisation */
    idx_t tmp = n;
    size_t fcnt = 0;
    idx_t fac_p[16];
    uint8_t fac_e[16];

    for (size_t i = 0; i < primes_cnt &&
         (uint64_t)primes[i] * primes[i] <= tmp; ++i)
    {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            fac_p[fcnt] = p;
            uint8_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            fac_e[fcnt] = e;
            ++fcnt;
        }
    }
    if (tmp > 1) {           /* remaining prime factor > sqrt(n) */
        fac_p[fcnt] = tmp;
        fac_e[fcnt] = 1;
        ++fcnt;
    }

    /* Estimate divisor count */
    size_t divsz = 1;
    for (size_t i = 0; i < fcnt; ++i)
        divsz *= (size_t)(fac_e[i] + 1);

    idx_t *arr = (idx_t *)malloc(divsz * sizeof(idx_t));
    size_t sz = 0;
    arr[sz++] = 1;           /* divisor 1 always present */

    for (size_t i = 0; i < fcnt; ++i) {
        idx_t p = fac_p[i];
        uint8_t e = fac_e[i];
        size_t cur_sz = sz;
        for (size_t j = 0; j < cur_sz; ++j) {
            idx_t cur = arr[j];
            for (uint8_t k = 1; k <= e; ++k) {
                cur *= p;
                arr[sz++] = cur;
            }
        }
    }

    qsort(arr, sz, sizeof(idx_t), cmp_idx);
    *divs = arr;
    return sz;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1; n=0 not used */

    /* Ensure we have primes up to sqrt(n) */
    uint32_t limit = (uint32_t)int_sqrt(n);
    if (limit < 2)
        limit = 2;
    ensure_primes(limit);

    /* Generate all divisors of n */
    idx_t *divs = NULL;
    size_t divc = compute_divisors(n, &divs);

    seq_t best = 0;
    bool first = true;

    /* Iterate over all triplets a ≤ b ≤ c with a·b·c = n */
    for (size_t i = 0; i < divc; ++i) {
        idx_t a_side = divs[i];
        for (size_t j = i; j < divc; ++j) {
            idx_t b_side = divs[j];
            /* If a_side * b_side would overflow or exceed n, break */
            if (a_side > n / b_side)
                break;
            idx_t ab = a_side * b_side;
            if (n % ab != 0)
                continue;
            idx_t c_side = n / ab;
            if (b_side > c_side)
                continue;

            seq_t sum = (seq_t)a_side * (seq_t)a_side
                      + (seq_t)b_side * (seq_t)b_side
                      + (seq_t)c_side * (seq_t)c_side;
            if (first || sum < best) {
                best = sum;
                first = false;
            }
        }
    }

    free(divs);
    return best;
}
