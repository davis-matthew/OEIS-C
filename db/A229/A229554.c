/*
 * A229554  a(n) = 7*n! + 1.
 * https://oeis.org/A229554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n! + 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t k = 2; k <= n; ++k) {
        fact *= (seq_t)k;
    }
    return fact * ((seq_t)7) + ((seq_t)1);
}
