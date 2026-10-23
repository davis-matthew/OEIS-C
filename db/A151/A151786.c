/*
 * A151786  a(n) = 8^(wt(n)-1) where wt(n) is the binary weight of n
 *          (A000120).
 * https://oeis.org/A151786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^(wt(n)-1) where wt(n) is the binary weight of n
 * Range:     u8 n <= 14, u16 n <= 126, u32 n <= 4094, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* count the number of 1 bits in n */
    idx_t w = 0;
    idx_t m = n;
    while (m) {
        m &= m - 1; /* drop the lowest set bit each iteration */
        w++;
    }

    /* compute 8^(w-1) */
    seq_t result = 1;
    seq_t base   = 8ULL;          /* base 8 */
    idx_t exp = w - 1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}
