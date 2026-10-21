/*
 * A094680  a(n+1) = 4*a(n)^3 - 3*a(n), with a(0) = 2.
 * https://oeis.org/A094680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(n+1) = 4*a(n)^3 - 3*a(n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool if needed */
#include <stddef.h>   /* for size_t, maybe idx_t defined elsewhere */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)2;
    }

    seq_t val = (seq_t)2;
    for (idx_t i = 1; i <= n; ++i) {
        /* compute 4*val^3 - 3*val */
        seq_t val2 = val * val;          /* val^2 */
        seq_t val3 = val2 * val;         /* val^3 */
        seq_t four_val3 = val3 * (seq_t)4;/* 4*val^3 */
        seq_t three_val = val * (seq_t)3;/* 3*val */
        val = four_val3 - three_val;
    }

    return val;
}
