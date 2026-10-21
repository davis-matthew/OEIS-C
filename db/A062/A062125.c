/*
 * A062125  Fifth column of A046741.
 * https://oeis.org/A062125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (40 + 126*n + 165*n^2 + 90*n^3 + 27*n^4) / 8
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 171, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 5, a(1) = 56, ... (polynomial derived from A062125) */
seq_t a(idx_t n)
{
    /* Convert index to sequence type for arithmetic */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */

    /* Compute the numerator of the polynomial */
    seq_t val = (seq_t)40;
    val += (seq_t)126 * nn;
    val += (seq_t)165 * n2;
    val += (seq_t)90  * n3;
    val += (seq_t)27  * n4;

    /* Divide by 8 (exact division) */
    return val / (seq_t)8;
}
