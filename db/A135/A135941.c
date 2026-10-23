/*
 * A135941  a(n) = floor(n/S2(n)), where S2(n) is the binary weight of n.
 * https://oeis.org/A135941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n / S2(n))
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* S2(n) is the binary weight (number of 1-bits) of n. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Count the number of 1-bits in n. */
    int w = 0;
    idx_t x = n;
    while (x) {
        x &= x - 1;   /* clear the lowest set bit */
        ++w;
    }

    /* w is at least 1 for n > 0 */
    idx_t res = n / (idx_t)w;
    return (seq_t)res;
}
