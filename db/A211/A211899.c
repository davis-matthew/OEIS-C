/*
 * A211899  Number of triangular n X n X n arrays colored with integers 0
 *          upwards introduced in row major order, with no element equal to
 *          any neighbor, and containing the value n(n+1)/2-2.
 * https://oeis.org/A211899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 2 n^3 - 13 n^2 + 10 n + 8)/8 for n > 1 a(1) = 0
 *            This implements the explicit polynomial that equals the 5-term
 *            recurrence a(n)=5a(n-1)-10a(n-2)+10a(n-3)-5a(n-4)+a(n-5)
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    /* Use a signed 128‑bit intermediate to avoid overflow and to
       correctly handle the subtractions before the final division. */
    __int128 N      = n;
    __int128 n2     = N * N;           /* n^2   */
    __int128 n3     = n2 * N;          /* n^3   */
    __int128 n4     = n3 * N;          /* n^4   */

    __int128 numerator = n4            /* n^4 */
                      + 2 * n3         /* + 2*n^3 */
                      - 13 * n2        /* -13*n^2 */
                      + 10 * N         /* +10*n  */
                      + 8;             /* +8     */

    __int128 result = numerator / 8;   /* exact division */
    return (seq_t)result;
}
