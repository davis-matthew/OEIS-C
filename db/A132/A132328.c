/*
 * A132328  a(n) = Product_{k>0} (1+floor(n/3^k)).
 * https://oeis.org/A132328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Product_{k>0} (1+floor(n/3^k))
 * Range:     u8 n <= 53, u16 n <= 242, u32 n <= 1781, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = (1 + floor(n/3)) * a(floor(n/3)), with a(0)=1 */
seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t m = n / 3;
    while (m > 0) {
        prod *= (seq_t)(m + 1);
        m /= 3;
    }
    return prod;
}
