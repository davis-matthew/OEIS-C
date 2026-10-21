/*
 * A240171  Numbers k such that k has more divisors than k-1.
 * https://oeis.org/A240171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A240171: numbers k such that divisor_count(k) > divisor_count(k-1) */

#include <stddef.h>
#include "../utils/divisor_count_u64.h"

/* A fixed-size cache is sufficient for the required range (n up to ≈10 000). */
#define CACHE_CAP 200000

seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t cache[CACHE_CAP];
    static size_t terms_cnt = 0;   /* number of terms already stored */
    static idx_t cur_k = 2;       /* next integer to test */

    /* Generate terms until we have at least n of them */
    while (terms_cnt < n)
    {
        /* avoid overflow: cur_k is idx_t (uint64_t) */
        seq_t d_k   = divisor_count_u64((uint64_t)cur_k);
        seq_t d_km1 = divisor_count_u64((uint64_t)(cur_k - 1));

        if (d_k > d_km1)
        {
            if (terms_cnt < CACHE_CAP)
                cache[terms_cnt++] = (seq_t)cur_k;
        }
        ++cur_k;
    }

    return cache[n - 1];
}
