/*
 * A027615  Number of 1's when n is written in base -2.
 * https://oeis.org/A027615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of 1's in the base –2 representation of n
 * Range:     u8 n <= 568, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p = (seq_t)n;   /* current positive part   */
    seq_t q = 0;          /* current negative part   */
    seq_t count = 0;      /* result */

    while (p | q) {                      /* stop when both zero */
        seq_t r = (p + q) & 1;           /* digit 0 or 1 */
        count += r;                      /* count 1's */

        /* next values:   N' = (q + r)/2  -  p/2   */
        seq_t p_next = (q + r) / 2;      /* new positive part   */
        seq_t q_next = p / 2;            /* new negative part   */

        p = p_next;
        q = q_next;
    }
    return count;
}
