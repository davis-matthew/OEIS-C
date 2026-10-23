/*
 * A116818  Number of permutations of length n which avoid the patterns 1234,
 *          2341, 3421.
 * https://oeis.org/A116818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n^7 - 119*n^6 + 651*n^5 - 1505*n^4 + 693*n^3 +
 *            6664*n^2 - 11436*n + 10080)/5040
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is undefined for the sequence; return 0 for safety */
    if (n == 0)
        return 0;

    __int128 N = n;
    __int128 n2 = N * N;          /* n^2 */
    __int128 n3 = n2 * N;         /* n^3 */
    __int128 n4 = n3 * N;         /* n^4 */
    __int128 n5 = n4 * N;         /* n^5 */
    __int128 n6 = n5 * N;         /* n^6 */
    __int128 n7 = n6 * N;         /* n^7 */

    __int128 num = 12 * n7
                 - 119 * n6
                 + 651 * n5
                 - 1505 * n4
                 + 693 * n3
                 + 6664 * n2
                 - 11436 * N
                 + 10080;

    num /= 5040;
    return (seq_t)num;
}
