/*
 * A160429  First differences of A160428.
 * https://oeis.org/A160429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8 * 7^A000120(n-1) (A000120 = binary weight of n-1)
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* offset starts at 1; undefined for n==0, return 0 */
        return 0;

    /* Count bits set in n-1 (A000120) */
    idx_t v = n - 1;
    unsigned int bitcnt = 0;
    while (v) {
        v &= v - 1; /* clear lowest set bit */
        ++bitcnt;
    }

    /* Compute 7^bitcnt as seq_t */
    seq_t result = (seq_t)1;
    for (unsigned int i = 0; i < bitcnt; ++i)
        result *= (seq_t)7;

    /* Multiply by 8 */
    result *= (seq_t)8;
    return result;
}
