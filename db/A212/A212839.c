/*
 * A212839  Number of 0..n arrays of length 6 with 0 never adjacent to n.
 * https://oeis.org/A212839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 6*n^5 + 5*n^4 - 12*n^3 - 3*n^2 + 6*n - 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned __int128 N = n;

    unsigned __int128 term6 = N * N * N * N * N * N;   /* n^6 */
    unsigned __int128 term5 = 6 * (N * N * N * N * N); /* 6·n^5 */
    unsigned __int128 term4 = 5 * (N * N * N * N);     /* 5·n^4 */
    unsigned __int128 term3 = 12 * (N * N * N);        /* 12·n^3 */
    unsigned __int128 term2 = 3 * (N * N);             /* 3·n^2 */

    unsigned __int128 result =
        term6 + term5 + term4 - term3 - term2 + 6 * N - 1;

    return (seq_t)result;
}
