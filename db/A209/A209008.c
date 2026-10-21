/*
 * A209008  Number of 4-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero and first and second differences in -n..n.
 * https://oeis.org/A209008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = (4*n^3 + 6*n^2 + 20*n + 48)/48 for n even, * a(n) =
 *            (4*n^3 + 6*n^2 + 20*n + 18)/48 for n odd. * This implements the
 *            closed‑form expression deduced from the generating * function
 *            and the explicit formulas given in the OEIS entry
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    seq_t val = 4 * n3 + 6 * n2 + 20 * nn;
    if (n & 1U)           /* odd */
        val += 18;
    else
        val += 48;

    return val / 48;
}
