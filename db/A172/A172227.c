/*
 * A172227  Number of ways to place 4 nonattacking wazirs on an n X n board.
 * https://oeis.org/A172227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8-30n^6+24n^5+323n^4-504n^3-1110n^2+2760n-1224)/24 for
 *            n>=3, a(1)=0, a(2)=0. Computes directly using signed __int128
 *            for intermediate values to avoid unsigned underflow. The result
 *            is cast to seq_t; for arbitrary‑precision builds the cast is
 *            performed by the compiler converting to the b...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 23, u64 n <= 380, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    /* compute all needed powers of n using 128‑bit signed integers */
    __int128 nn = n;
    __int128 n2 = nn * nn;          /* n^2 */
    __int128 n4 = n2 * n2;          /* n^4 */
    __int128 n3 = n4 / nn;          /* n^3 (use n4/n to avoid overflow) */
    __int128 n5 = n4 * nn;          /* n^5 */
    __int128 n6 = n4 * n2;          /* n^6 */
    __int128 n8 = n4 * n4;          /* n^8 */

    /* assemble the numerator of the formula */
    __int128 num =
        n8
        - 30 * n6
        + 24 * n5
        + 323 * n4
        - 504 * n3
        - 1110 * n2
        + 2760 * nn
        - 1224;

    /* The polynomial is divisible by 24 for all integer n>=3. */
    num /= 24;

    return (seq_t)num;
}
