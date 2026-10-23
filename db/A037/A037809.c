/*
 * A037809  Number of i such that d(i) <= d(i-1), where Sum_{i=0..m} d(i)*2^i
 *          is the base-2 representation of n.
 * https://oeis.org/A037809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of indices i >= 1 in the binary expansion of n
 *            such that the i‑th bit (counting from 0 as the least significant
 *            bit) is <= the (i-1)‑th bit
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* Treat n == 0 as a special case (offset start is 1 but code is
       robust for all n >= 0). */
    if (n == 0)
        return 0;

    /* previous bit = least significant bit */
    idx_t prev = n & 1;
    n >>= 1;

    while (n) {
        idx_t curr = n & 1;
        if (curr <= prev)
            ++count;
        prev = curr;
        n >>= 1;
    }

    return count;
}
