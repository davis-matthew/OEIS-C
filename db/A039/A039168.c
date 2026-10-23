/*
 * A039168  Numbers whose base-11 representation has the same number of 1's
 *          and 5's.
 * https://oeis.org/A039168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th nonnegative integer whose base‑11 representation
 *            * contains the same number of 1's and 5's
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety – valid for any n ≥ 1 */

    seq_t cand = (seq_t)0;
    idx_t  count = 0;

    for (;;) {
        seq_t tmp   = cand;
        int   c1    = 0, c5 = 0;

        /* Count 1's and 5's in the base‑11 representation of cand */
        while (tmp != 0) {
            seq_t digit = tmp % 11;
            if (digit == 1) ++c1;
            else if (digit == 5) ++c5;
            tmp = tmp / 11;
        }

        if (c1 == c5) {   /* cand satisfies the condition */
            ++count;
            if (count == n) return cand;
        }

        cand = cand + 1;   /* try next integer */
    }
}
