/*
 * A039153  Numbers whose base-10 representation has the same number of 7's
 *          and 8's.
 * https://oeis.org/A039153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-10 representation has the same number of 7's and 8's.
   Brute‑force search: iterate over integers, count digits, and collect
   those with equal counts of 7 and 8. */
seq_t a(idx_t n)
{
    idx_t m = 0;          /* current integer being tested   */
    idx_t found = 0;      /* how many valid numbers seen     */

    while (found < n) {
        seq_t v = (seq_t)m;
        int cnt7 = 0, cnt8 = 0;

        /* Count 7's and 8's in v */
        while (v > 0) {
            seq_t r = v % 10;
            if (r == 7)      cnt7++;
            else if (r == 8) cnt8++;
            v = v / 10;
        }

        if (cnt7 == cnt8) {
            found++;              /* this m is a valid term   */
        }
        m++;                       /* move to next integer    */
    }

    return (seq_t)(m - 1);   /* m was incremented once past the desired term */
}
