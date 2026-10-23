/*
 * A197629  Number of ways to write n as the sum of two coprime, squarefree,
 *          composite, positive integers.
 * https://oeis.org/A197629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 601, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A197629
 * a(n) = number of unordered pairs (x, y) of coprime, square‑free, composite
 * positive integers with x + y = n (n ≥ 1).
 * The function computes the value by precomputing smallest prime factors
 * and the square‑free/composite flag only once (lazy, thread‑unsafe).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Smallest prime factor table: spf[i] = smallest prime factor of i (i ≥ 2). */
static uint32_t *spf = NULL;
/* Flag for composite & square‑free numbers. */
static bool *is_cf = NULL;
/* Largest index for which tables are currently computed. */
static idx_t premax = 0;

/* Integer square root of a 64‑bit unsigned integer (floor). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Allocate and populate spf and is_cf tables up to `limit` (inclusive). */
static void build_pre(idx_t limit)
{
    if (limit <= premax) return;          /* already built */

    /* free old tables, if any */
    free(spf);
    free(is_cf);

    spf  = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (!spf) return;
    is_cf = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_cf) {
        free(spf);
        spf = NULL;
        return;
    }

    /* initialise spf with own value (will be overwritten for composites) */
    for (idx_t i = 2; i <= limit; ++i) spf[i] = (uint32_t)i;

    /* sieve to set smallest prime factor for composites */
    idx_t r = isqrt_u64(limit);
    for (idx_t i = 2; i <= r; ++i) {
        if (spf[i] != (uint32_t)i) continue;          /* not prime */
        for (idx_t j = i * i; j <= limit; j += i) {
            if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }

    /* compute composite & square‑free flag */
    for (idx_t x = 2; x <= limit; ++x) {
        bool composite = (spf[x] != (uint32_t)x);

        bool squarefree = true;
        uint64_t tmp = x;
        /* factorise once, checking for repeated prime factors */
        while (tmp > 1) {
            uint32_t p = spf[(idx_t)tmp];
            tmp /= p;
            if (tmp % p == 0) {            /* p appears ≥ 2 times */
                squarefree = false;
                break;
            }
        }

        is_cf[x] = composite && squarefree;
    }

    premax = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset starts at 1 */

    /* ensure tables cover all numbers up to n */
    build_pre(n);

    seq_t count = 0;
    for (idx_t x = 2; x <= n / 2; ++x) {
        if (!is_cf[x]) continue;
        idx_t y = n - x;
        if (!is_cf[y]) continue;
        if (gcd((seq_t)x, (seq_t)y) == 1) ++count;
    }
    return count;
}
