/*
 * A132272  a(n) = Product_{k>0} (1 + floor(n/10^k)).
 * https://oeis.org/A132272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Product_{k>0} (1 + floor(n/10^k))
 * Range:     u8 n <= 499, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    while (n > 0)
    {
        n /= 10;                    /* floor(n/10^k) for current k */
        prod *= (seq_t)(1 + (seq_t)n);
    }
    return prod;
}
