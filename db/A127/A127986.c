/*
 * A127986  a(n) = n! + 2^n - 1.
 * https://oeis.org/A127986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! + 2^n - 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact  = (seq_t)1;
    for (idx_t i = (idx_t)2; i <= n; ++i)
        fact = fact * (seq_t)i;

    seq_t power = (seq_t)1;
    for (idx_t i = (idx_t)0; i < n; ++i)
        power = power * (seq_t)2;

    return fact + power - (seq_t)1;
}
