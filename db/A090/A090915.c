/*
 * A090915  Permutation of natural numbers arising from a square spiral.
 * https://oeis.org/A090915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Let r be the smallest positive integer with 1+4r(r+1) ≥ n. Then
 *            let prev = 1+4(r-1)r (the number of positions before ring r),
 *            let num = 1+4r(r+1) (the largest number in ring r). For n ==
 *            prev+8r (the last position in the ring) a(n) = num; otherwise
 *            a(n) = num - (n - prev)
 * Range:     u8 n <= 225, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the center */
    if (n == 1)
        return (seq_t)1;

    /* find the ring containing n */
    idx_t r = 1;
    while (1 + 4 * r * (r + 1) < n)
        ++r;

    /* count of indices up to the previous ring */
    idx_t prev = 1 + 4 * (r - 1) * r;

    /* largest number in the current ring */
    idx_t num = 1 + 4 * r * (r + 1);

    /* offset of n inside this ring (1 … 8r) */
    idx_t offset = n - prev;

    /* last position of the ring maps to the maximum number */
    if (offset == 8 * r)
        return (seq_t)num;

    /* otherwise the value decreases by the offset */
    return (seq_t)(num - offset);
}
