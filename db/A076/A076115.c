/*
 * A076115  Squares (or 0) from A076114.
 * https://oeis.org/A076115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A076115:  a(n)  = the smallest square that is the sum of n
 *  consecutive positive integers; 0 if such a square does not exist.
 */
#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;            /* undefined, guard */

    /* C = n*(n-1)/2  (sum of the first n numbers minus n) */
    seq_t n_seq = (seq_t)n;
    seq_t C = n_seq * (n_seq - (seq_t)1) / (seq_t)2;

    /* residue r = C mod n  (optimized to avoid big intermediate) */
    seq_t r_seq = ((n % (idx_t)2) == 0) ? (seq_t)(n / 2) : (seq_t)0;

    /* minimal y whose square is at least C + n  */
    seq_t target  = C + n_seq;                     /* C + n       */
    seq_t y_min   = floor_sqrt(target);            /* floor sqrt  */
    if (y_min * y_min < target) ++y_min;           /* ceil sqrt   */

    uint64_t y_min_mod = (uint64_t)(y_min % n_seq);

    seq_t best_y = (seq_t)-1;                      /* sentinel     */

    for (uint64_t y_mod = 0; y_mod < (uint64_t)n; ++y_mod)
    {
        /* check whether y_mod^2 ≡ r (mod n) */
        if ( ((seq_t)y_mod * (seq_t)y_mod) % n_seq == r_seq )
        {
            /* smallest y ≥ y_min with y ≡ y_mod (mod n) */
            uint64_t delta = (y_mod + (uint64_t)n - y_min_mod) % (uint64_t)n;
            seq_t y = y_min + (seq_t)delta;

            if (best_y == (seq_t)-1 || y < best_y)
                best_y = y;
        }
    }

    if (best_y == (seq_t)-1)           /* no solution */
        return (seq_t)0;

    return best_y * best_y;            /* the square itself */
}
