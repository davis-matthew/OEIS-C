/*
 * A211906  Number of lower triangular n X n arrays colored with integers 0
 *          upwards introduced in row major order, with no element equal to
 *          any horizontal or vertical neighbor, and containing the value
 *          n(n+1)/2-3.
 * https://oeis.org/A211906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((3*n^8 + 12*n^7 - 58*n^6 - 120*n^5 + 579*n^4 - 100*n^3 -
 *            1100*n^2 + 784*n + 384) / 384) for n > 1 a(1) = 0 This formula
 *            is exact for all n ≥ 1 and gives the integer sequence A211906
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1u)
        return (seq_t)0;

    /* Use a wider signed type for intermediate arithmetic to avoid
       overflow and to handle the subtractions correctly. */
    __int128 N   = (__int128)n;
    __int128 n2  = N * N;          /* n^2 */
    __int128 n3  = n2 * N;         /* n^3 */
    __int128 n4  = n3 * N;         /* n^4 */
    __int128 n5  = n4 * N;         /* n^5 */
    __int128 n6  = n5 * N;         /* n^6 */
    __int128 n7  = n6 * N;         /* n^7 */
    __int128 n8  = n7 * N;         /* n^8 */

    __int128 num =  3 * n8
                  + 12 * n7
                  - 58 * n6
                  - 120 * n5
                  + 579 * n4
                  - 100 * n3
                  - 1100 * n2
                  + 784 * N
                  + 384;

    /* The numerator is always divisible by 384 for n > 1 */
    return (seq_t)(num / 384);
}
