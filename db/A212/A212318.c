/*
 * A212318  Expansion of phi(q^2)^2 / (phi(-q) * phi(q^4)) in powers of q
 *          where phi() is a Ramanujan theta function.
 * https://oeis.org/A212318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 74, u64 n <= 255, u128 n <= 806,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A212318   Expansion of φ(q²)² / (φ(−q) φ(q⁴))
 *
 *  The coefficients a(n) satisfy the recurrence
 *
 *      a(0) = 1
 *      a(n) = (1/n) · Σ_{k=1}^{n} d(k) · a(n−k)          (n≥1)
 *
 *  where d(k) = Σ_{d|k} d·b(d) and the periodic sequence b(k)
 *  has period 16:
 *
 *      b(1)= 2, b(2)= 5, b(3)= 2, b(4)= −7,
 *      b(5)= 2, b(6)= 5, b(7)= 2, b(8)= 2,
 *      b(9)= 2, b(10)=5, b(11)=2, b(12)=−7,
 *      b(13)=2, b(14)=5, b(15)=2, b(16)= 0, ...
 *
 *  The recurrence is valid for all n≥0; for n=0 the term is 1.
 *
 *  The implementation stores the sequence in a cache and expands it
 *  on demand.  To avoid overflow when seq_t is 64‑bit, an intermediate
 *  unsigned 128‑bit accumulator is used.  For larger seq_t types
 *  ordinary arithmetic of seq_t suffices.
 */

#include <stdlib.h>
#include <stdint.h>

static const int period[16] = {
    2, 5, 2, -7,
    2, 5, 2, 2,
    2, 5, 2, -7,
    2, 5, 2, 0
};

seq_t a(idx_t n)
{
    /* cache of a values */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;

    /* cache of d(k) values */
    static int64_t *d = NULL;
    static size_t d_sz = 0;

    /* Initialise cache for a(0) */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache_sz = 1;
    }

    /* Expand cache and d array if necessary */
    if (cache_sz <= (size_t)n) {
        size_t new_sz = (size_t)n + 1;

        /* Reallocate cache */
        seq_t *new_cache = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;
        cache = new_cache;

        /* Reallocate d */
        int64_t *new_d = (int64_t*)realloc(d, new_sz * sizeof(int64_t));
        if (new_d == NULL) return (seq_t)0;
        d = new_d;

        /* Compute d(k) for all 1 ≤ k < new_sz  */
        for (size_t k = 1; k < new_sz; ++k) d[k] = 0;
        for (size_t divisor = 1; divisor < new_sz; ++divisor) {
            int val = divisor * period[(divisor - 1) & 15];
            if (val == 0) continue;
            for (size_t m = divisor; m < new_sz; m += divisor) {
                d[m] += (int64_t)val;
            }
        }

        /* Compute new a values */
        for (size_t i = cache_sz; i < new_sz; ++i) {
            /* Use 128‑bit accumulator when seq_t is 8 bytes */
            if (sizeof(seq_t) <= 8) {
                __uint128_t sum = 0;
                for (size_t k = 1; k <= i; ++k) {
                    uint64_t ck_abs = (uint64_t)(d[k] >= 0 ? d[k] : -d[k]);
                    __uint128_t prod = (__uint128_t)cache[i - k] * ck_abs;
                    if (d[k] >= 0) sum += prod; else sum -= prod;
                }
                cache[i] = (seq_t)(sum / (uint64_t)i);
            } else { /* seq_t larger than 64 bits */
                seq_t sum = 0;
                for (size_t k = 1; k <= i; ++k) {
                    seq_t ck_abs = (seq_t)(d[k] >= 0 ? d[k] : -d[k]);
                    seq_t prod = cache[i - k] * ck_abs;
                    if (d[k] >= 0) sum += prod; else sum -= prod;
                }
                cache[i] = sum / (seq_t)i;
            }
        }
        cache_sz = new_sz;
    }

    return cache[(size_t)n];
}
