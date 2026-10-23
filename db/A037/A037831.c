/*
 * A037831  Number of i such that d(i) >= d(i-1), where Sum_{i=0..m} d(i)*8^i
 *          is the base-8 representation of n.
 * https://oeis.org/A037831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   number of indices i ≥ 1 such that the i‑th base‑8 digit
 *            (counting from the least significant digit d(0)) satisfies d(i)
 *            ≥ d(i – 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined by the OEIS, but safe */

    seq_t count = 0;
    idx_t temp = n;

    /* first (least significant) digit – no comparison for it */
    unsigned char prev = (unsigned char)(temp & 7);
    temp >>= 3;                    /* move to next digit */

    while (temp > 0) {
        unsigned char curr = (unsigned char)(temp & 7);
        if (curr >= prev) ++count;
        prev = curr;
        temp >>= 3;
    }

    return count;
}
