/*
 * A048924  9-gonal octagonal numbers.
 * https://oeis.org/A048924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 454275*a(n-1) - 454275*a(n-2) + a(n-3) (n ≥ 4) Initial
 *            values: a(1) = 1, a(2) = 631125, a(3) = 286703855361 The
 *            sequence is computed directly from the recurrence; no caching is
 *            necessary for the required limits
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)631125;
    if (n == 3) return (seq_t)286703855361;

    seq_t prev3 = (seq_t)1;                /* a(n-3)   */
    seq_t prev2 = (seq_t)631125;           /* a(n-2)   */
    seq_t prev1 = (seq_t)286703855361;     /* a(n-1)   */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur = ( (seq_t)454275 * prev1 )
            - ( (seq_t)454275 * prev2 )
            + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
