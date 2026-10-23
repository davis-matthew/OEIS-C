/*
 * A027966  T(n, 2*n-4), T given by A027960.
 * https://oeis.org/A027966
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 2*n^3 - 25*n^2 + 94*n - 96)/24
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 276, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* computed directly using integer arithmetic. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* values below offset are irrelevant */
    seq_t nn = (seq_t)n;
    seq_t n2  = nn * nn;            /* n^2 */
    seq_t n3  = n2 * nn;            /* n^3 */
    seq_t n4  = n3 * nn;            /* n^4 */
    seq_t num = n4 + 2 * n3 - 25 * n2 + 94 * nn - 96;
    return num / 24;
}
