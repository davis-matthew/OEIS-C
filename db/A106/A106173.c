/*
 * A106173  7th diagonal of triangle in A059317.
 * https://oeis.org/A106173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 39*n^5 - 155*n^4 - 2655*n^3 + 20674*n^2 - 51744*n
 *            + 42480)/720 for n > 3, a(0)=a(1)=a(2)=a(3)=0
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;

    /* Use signed 128‑bit to avoid overflow in intermediate products. */
    signed __int128 N = n;
    signed __int128 n2 = N * N;      /* n^2 */
    signed __int128 n3 = n2 * N;     /* n^3 */
    signed __int128 n4 = n3 * N;     /* n^4 */
    signed __int128 n5 = n4 * N;     /* n^5 */
    signed __int128 n6 = n5 * N;     /* n^6 */

    signed __int128 num = n6
                         + 39 * n5
                         - 155 * n4
                         - 2655 * n3
                         + 20674 * n2
                         - 51744 * N
                         + 42480;

    num /= 720;

    return (seq_t)num;
}
