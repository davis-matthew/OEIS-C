/*
 * A100172  Structured triakis icosahedral numbers (vertex structure 4).
 * https://oeis.org/A100172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (57*n^3 - 81*n^2 + 30*n)/6
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Promote n to seq_t before any arithmetic to avoid premature truncation */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t num = 57 * n3 - 81 * n2 + 30 * nn; /* numerator */
    return num / 6;               /* integer division; numerator always divisible by 6 */
}
