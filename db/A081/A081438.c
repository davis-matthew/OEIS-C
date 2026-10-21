/*
 * A081438  Diagonal in array of n-gonal numbers A081422.
 * https://oeis.org/A081438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 9*n^2 + 9*n + 2) / 2
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                      /* cast the index to seq_t */
    unsigned __int128 n128 = (unsigned __int128)nn;   /* promote for 128‑bit arithmetic */

    unsigned __int128 num =
          (unsigned __int128)2 * n128 * n128 * n128
        + (unsigned __int128)9 * n128 * n128
        + (unsigned __int128)9 * n128
        + (unsigned __int128)2;

    num /= 2;                                 /* exact division, result fits in seq_t when it fits */

    return (seq_t)num;
}
