/*
 * A207401  Number of n X 6 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 12*n^5 + 58*n^4 + 120*n^3 + 49*n^2 - 132*n + 36) /
 *            9
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 49, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1)-21*a(n-2)+35*a(n-3)-35*a(n-4)+21*a(n-5)-7*a(n-6)+a(n-7) for n>7 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;       /* n^2 */
    seq_t N3 = N2 * N;      /* n^3 */
    seq_t N4 = N3 * N;      /* n^4 */
    seq_t N5 = N4 * N;      /* n^5 */
    seq_t N6 = N5 * N;      /* n^6 */

    seq_t num = N6
                + 12 * N5
                + 58 * N4
                + 120 * N3
                + 49 * N2
                - 132 * N
                + 36;

    return num / 9;
}
