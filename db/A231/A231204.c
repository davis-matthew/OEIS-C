/*
 * A231204  If n = Sum_{i=0..m} c(i)*2^i, c(i) = 0 or 1, then a(n) =
 *          Sum_{i=0..m} (m-i)*c(i).
 * https://oeis.org/A231204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{i=0}^{m} (m-i) * c(i), where c(i) is the i‑th * bit of
 *            the binary representation of n and m = ⌊log₂ n⌋. * For n = 0 the
 *            sum is 0
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Find index of most significant set bit (msb). */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp > 1)
    {
        tmp >>= 1;
        ++msb;
    }

    /* Sum contributions of each 1‑bit. */
    seq_t sum = 0;
    for (idx_t i = 0; i <= msb; ++i)
    {
        if (n & ((idx_t)1 << i))
            sum += (seq_t)(msb - i);
    }
    return sum;
}
