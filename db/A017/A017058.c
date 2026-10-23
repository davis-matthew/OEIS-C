/*
 * A017058  a(n) = (7*n + 6)^6.
 * https://oeis.org/A017058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 6)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 4, u64 n <= 231, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to the sequence type first to avoid overflow in intermediate */
    seq_t base = ((seq_t)n) * 7 + 6;
    seq_t result = base;
    /* Compute the sixth power by successive multiplications */
    for (int i = 0; i < 5; ++i) {
        result *= base;
    }
    return result;
}
