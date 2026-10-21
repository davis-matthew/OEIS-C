/*
 * A080775  Number of n X n monomial matrices whose nonzero entries are unit
 *          Hurwitz quaternions.
 * https://oeis.org/A080775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24^n * n!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        result *= (seq_t)24 * (seq_t)k;
    }
    return result;
}
