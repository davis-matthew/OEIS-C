/*
 * A147993  Sequence S such that 1 is in S and if x is in S, then 6x-1 and
 *          6x+1 are in S.
 * https://oeis.org/A147993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 Recurrence (BFS numbering): a(2i) = 6*a(i) - 1
 *            Recurrence (BFS numbering): a(2i+1) = 6*a(i) + 1 (i ≥ 1) This
 *            yields the sequence A147993 in increasing order
 * Range:     u8 n <= 13, u16 n <= 127, u32 n <= 8191, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The root value. */
    seq_t val = 1;
    if (n <= 1)
        return val;

    /* Find the position of the most‑significant 1‑bit of n.
       For n>1 this is floor(log2(n)). */
    int msb = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++msb;
    }

    /* Walk the bits of n from the second‑most significant one down to
       the least significant.  For each 0 we go left (6*val-1),
       for each 1 we go right (6*val+1). */
    for (int i = msb - 1; i >= 0; --i) {
        if (n & ((idx_t)1 << i))
            val = (seq_t)6 * val + 1;
        else
            val = (seq_t)6 * val - 1;
    }
    return val;
}
