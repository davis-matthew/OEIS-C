/*
 * A207166  Number of n X 5 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (13*n^4 + 26*n^3 + 117*n^2 - 104*n)/4
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 151, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;         /* Use seq_t for all arithmetic to match the build type */
    seq_t N2 = N * N;          /* n² */
    seq_t N3 = N2 * N;         /* n³ */
    seq_t N4 = N3 * N;         /* n⁴ */

    seq_t num = 13 * N4 + 26 * N3 + 117 * N2 - 104 * N;
    return num / 4;            /* Integer division – the numerator is always divisible by 4 */
}
