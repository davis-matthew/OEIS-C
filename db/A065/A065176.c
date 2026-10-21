/*
 * A065176  Site swap sequence associated with the permutation A065174 of Z.
 * https://oeis.org/A065176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 0, a(n) = 2 * (floor(n/2) & -floor(n/2)) for n >= 2 (i.e.
 *            a(n) = 2 * (n/2 & -(n/2)))
 * Range:     u8 n <= 255, u16 n <= 9803, u32 n <= 9803, u64 n <= 9803,
 *            u128 n <= 9803, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A065176: site‑swap sequence associated with A065174 of Z. */
seq_t a(idx_t n)
{
    /* Special value at the offset. */
    if (n <= 1)
        return 0;

    /* Convert the index to the element‑type before arithmetic. */
    seq_t k = (seq_t)n;   /* n as seq_t */
    k >>= 1;              /* k = floor(n/2) */

    /* Lowest set bit of k:  k & -k  is the greatest power of two dividing k. */
    seq_t low = k & (-k);

    /* The desired value is twice that power of two. */
    return low << 1;
}
