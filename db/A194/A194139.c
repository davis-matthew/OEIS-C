/*
 * A194139  a(n) = Sum_{j=1..n} floor(j*(2+sqrt(2))); n-th partial sum of
 *          Beatty sequence for 2+sqrt(2).
 * https://oeis.org/A194139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} floor((j+1)*(2+sqrt(2)))
 * Range:     u8 n <= 4, u16 n <= 57, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 62 terms through n = 4605 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    static seq_t *sums = NULL;
    static idx_t maxk = 0;      /* number of terms already computed */
    static size_t cap = 0;      /* allocated capacity */

    idx_t k = n + 1;            /* need sum of the first k terms */
    if (k > maxk) {
        size_t needed = (size_t)(k + 1);
        size_t new_cap = cap;
        if (!new_cap) new_cap = 64;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, sums, new_cap);
        if (!tmp) return 0;    /* allocation failed */
        sums = tmp;
        cap = new_cap;
        if (maxk == 0)         /* initialise the base case */
            sums[0] = 0;
    }

    for (idx_t j = maxk + 1; j <= k; ++j) {
        seq_t jj   = (seq_t)j;
        seq_t term = 2 * jj + floor_sqrt(jj * jj * 2);
        sums[j] = sums[j - 1] + term;
    }
    maxk = k;
    return sums[k];
}
