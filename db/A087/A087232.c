/*
 * A087232  a(n) is the largest odd term in the 3x+1 trajectory initiated at
 *          n.
 * https://oeis.org/A087232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the largest odd term in the 3x+1 trajectory starting
 *            from n
 * Range:     u8 n <= 26, u16 n <= 702, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t maxodd = 0;
    while (1) {
        if ((x & 1) != 0) {
            if (x > maxodd)
                maxodd = x;
        }
        if (x == (seq_t)1)
            break;
        if ((x & 1) == 0) {
            x >>= 1;
        } else {
            x = 3 * x + 1;
        }
    }
    return maxodd;
}
