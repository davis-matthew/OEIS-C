/*
 * A060806  Denominators of special continued fraction for 2*zeta(3).
 * https://oeis.org/A060806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(4*k+1) = 2*k+2; a(4*k+2) = 2*k+4; a(4*k+3) = 2*k+3; a(4*k+4) =
 *            2*k+2
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the block index k and the offset inside the block r */
    idx_t k = (n - 1) / 4;
    idx_t r = (n - 1) % 4;

    switch (r) {
        case 0: /* n = 4*k+1 */
            return (seq_t)(2 * k + 2);
        case 1: /* n = 4*k+2 */
            return (seq_t)(2 * k + 4);
        case 2: /* n = 4*k+3 */
            return (seq_t)(2 * k + 3);
        case 3: /* n = 4*k+4 */
            return (seq_t)(2 * k + 2);
    }
    /* Unreachable for n >= 1 */
    return (seq_t)0;
}
