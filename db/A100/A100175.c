/*
 * A100175  Structured triakis tetrahedral numbers (vertex structure 4).
 * https://oeis.org/A100175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*n^3 - 3*n^2 + 2*n)/2
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type first */
    seq_t nn = (seq_t)n;

    /* Compute n^2 and n^3 using seq_t arithmetic */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    /* Apply the formula, using only integer operations */
    seq_t val = (3 * n3 - 3 * n2 + 2 * nn) / 2;

    return val;
}
