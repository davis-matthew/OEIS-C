/*
 * A216762  a(n) = n * ceiling(log_2(n)) * ceiling(log_2(log_2(n))) *
 *          ceiling(log_2(log_2(log_2(n)))) ....
 * https://oeis.org/A216762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ceil(log_2(n)) * ceil(log_2(log_2(n))) * ... while
 *            the result > 1
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* safety: a(0) = 1 by definition */

    seq_t prod = (seq_t)n;            /* start with n itself      */
    idx_t m    = n;                   /* current argument for log */

    while (1) {
        if (m <= 1)                   /* stop when ceil(log₂(m)) ≤ 1 */
            break;

        idx_t fl      = floor_log2_uint64(m);
        idx_t ceil_l  = (m == ((idx_t)1 << fl)) ? fl : fl + 1;

        if (ceil_l <= 1)
            break;

        prod *= (seq_t)ceil_l;        /* multiply by the current factor */
        m    = ceil_l;                /* move to the next level          */
    }
    return prod;
}
