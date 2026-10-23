/*
 * A152881  Positions of those 1's that are followed by a 0, summed over all
 *          Fibonacci binary words of length n. A Fibonacci binary word is a
 *          binary word having no 00 subword.
 * https://oeis.org/A152881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of positions of 1's followed by 0 in all Fibonacci
 *            binary words of length n. Computed by dynamic programming; see
 *            comments in the code
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n == 0, return 0 */
    /* counts of words ending in 0 or 1 */
    seq_t count0 = 1;              /* length 1 word "0" */
    seq_t count1 = 1;              /* length 1 word "1" */
    /* total positions sum for words ending in 0 or 1 */
    seq_t sum0   = 0;              /* no positions in length 1 words */
    seq_t sum1   = 0;
    /* iterate length from 2 to n */
    for (idx_t len = 2; len <= n; ++len) {
        idx_t pos = len - 1;                     /* position of previous bit */
        seq_t new_count0 = count1;               /* new word ends with 0: prev must be 1 */
        seq_t new_sum0   = sum1 + count1 * (seq_t)pos; /* add position pos for each such word */

        seq_t new_count1 = count0 + count1;      /* new word ends with 1: prev can be 0 or 1 */
        seq_t new_sum1   = sum0 + sum1;          /* no new positions added */

        count0 = new_count0; sum0 = new_sum0;
        count1 = new_count1; sum1 = new_sum1;
    }
    return sum0 + sum1;                       /* total over both endings */
}
