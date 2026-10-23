/*
 * A130652  a(n) = 11^n - 2.
 * https://oeis.org/A130652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + 20; a(1) = 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined below the offset */

    seq_t val = (seq_t)9;           /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        val = val * (seq_t)11 + (seq_t)20;
    }
    return val;
}
