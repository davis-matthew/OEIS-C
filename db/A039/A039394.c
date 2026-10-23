/*
 * A039394  Numbers whose base-10 representation has the same nonzero number
 *          of 0's and 2's.
 * https://oeis.org/A039394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A039394.
   Numbers whose base‑10 representation has the same nonzero number
   of 0's and 2's.
   The function scans successive integers, counting digits 0 and 2
   for each candidate, and returns the first that satisfies the
   condition.  It handles arbitrary seq_t sizes. */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* undefined for offset 0, but return 0 */
    idx_t found = 0;
    seq_t x = 1;                   /* start from integer 1 */
    const seq_t TEN = (seq_t)10;

    for (;;) {
        /* Count zeros and twos in x */
        seq_t tmp = x;
        unsigned cnt0 = 0, cnt2 = 0;
        while (tmp > 0) {
            int d = (int)(tmp % TEN);
            if (d == 0) cnt0++;
            else if (d == 2) cnt2++;
            tmp /= TEN;
        }
        if (cnt0 == cnt2 && cnt0 > 0) {   /* at least one of each */
            found++;
            if (found == n) return x;
        }
        x++;  /* move to next integer */
    }
}
