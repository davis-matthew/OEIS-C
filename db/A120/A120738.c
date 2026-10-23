/*
 * A120738  a(n) = 4*n - A000120(n).
 * https://oeis.org/A120738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*n - A000120(n) , where A000120(n) is the popcount of n
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute popcount of n (n <= 64 bits) */
    uint64_t m = (uint64_t)n;
    uint64_t popcnt = 0;
    while (m)
    {
        m &= m - 1;      /* clear lowest set bit */
        popcnt++;
    }

    /* compute 4*n - popcnt */
    seq_t result = ((seq_t)n << 2) - (seq_t)popcnt;
    return result;
}
