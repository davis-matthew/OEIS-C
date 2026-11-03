/*
 * A119346  Sequence of nim-values for the game in which two players
 *          alternately cut off one inch or root two inches from a piece of
 *          string of length n. Player who runs out of string loses.
 * https://oeis.org/A119346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n mod (1 + sqrt(2)))
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of nim-values for the game in which two players alternately cut off 1
   inch or sqrt(2) inches from a piece of string of length n. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

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
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(0)=0, a(1)=1, a(2)=2 are trivial */
    if (n <= (idx_t)2)
        return (seq_t)n;

    seq_t n_seq = (seq_t)n;

    /* fm_n = floor(n * sqrt(2)) = floor(sqrt(2 * n^2)) */
    seq_t fm_n = floor_sqrt((seq_t)2 * n_seq * n_seq);

    /* k = floor(n * (sqrt(2)-1)) = fm_n - n */
    seq_t k = fm_n - n_seq;

    /* For small n where k==0 we have a(n)=n */
    if (k == (seq_t)0)
        return n_seq;

    /* fm_k = floor(k * sqrt(2)) = floor(sqrt(2 * k^2)) */
    seq_t fm_k = floor_sqrt((seq_t)2 * k * k);

    /* a(n) = n - k - floor(k * sqrt(2)) - 1  (when k>0) */
    return n_seq - k - fm_k - (seq_t)1;
}
