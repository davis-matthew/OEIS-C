/*
 * A145441  Exponents of multipliers 10^a(n) of SI prefixes, in increasing
 *          order.
 * https://oeis.org/A145441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = n (n <= 3) * a(n) = 3*(n-2) (n >= 4)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)n;
    return (seq_t)3 * ((seq_t)n - 2);
}
