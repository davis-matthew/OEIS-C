/*
 * A028982  Squares and twice squares.
 * https://oeis.org/A028982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 435, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: sorted union of squares and twice squares */

seq_t a(idx_t n)
{
    /* Return the n-th smallest integer that is either a perfect square
       or twice a perfect square.  The sequence is the sorted merge of
       {k^2 | k ≥ 1} and {2*k^2 | k ≥ 1}. */
    if (n == 0)
        return (seq_t)0;                /* undefined for n<1, but harmless */

    idx_t i = (idx_t)1;                /* index for squares */
    idx_t j = (idx_t)1;                /* index for twice‑squares */
    seq_t sq_val  = (seq_t)1 * (seq_t)1;   /* 1^2 */
    seq_t tsq_val = (seq_t)2 * (seq_t)1 * (seq_t)1; /* 2*1^2 */
    idx_t cnt = 0;
    seq_t result = 0;

    while (cnt < n) {
        if (sq_val <= tsq_val) {
            result = sq_val;
            ++i;
            sq_val = (seq_t)i * (seq_t)i;
        } else {
            result = tsq_val;
            ++j;
            tsq_val = (seq_t)2 * (seq_t)j * (seq_t)j;
        }
        ++cnt;
    }
    return result;
}
