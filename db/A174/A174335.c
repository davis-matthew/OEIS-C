/*
 * A174335  Upper bound in enumerating what majority decisions are possible
 *          with possible abstaining.
 * https://oeis.org/A174335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 16*(n^3)*(n!)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* Compute n^3 */
    seq_t n3 = (seq_t)n * (seq_t)n * (seq_t)n;

    /* Compute 16 * n^3 * n! */
    seq_t result = fact * n3;
    result *= (seq_t)16;

    return result;
}
