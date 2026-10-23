/*
 * A039173  Numbers whose base-11 representation has the same number of 1's
 *          and 10's.
 * https://oeis.org/A039173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A039173: Numbers whose base‑11 representation has the same
   number of 1's and 10's.
   This function enumerates integers in increasing order, checks the
   digit counts in base 11, and returns the n‑th such number. */
seq_t a(idx_t n)
{
    idx_t   counted = 0;   /* how many valid numbers found so far     */
    seq_t   val     = 0;    /* current candidate integer              */

    while (counted < n) {
        /* Count digits 1 and 10 in the base‑11 representation of val. */
        seq_t   tmp   = val;
        unsigned cnt1  = 0, cnt10 = 0;

        while (tmp > 0) {
            seq_t q = tmp / (seq_t)11;
            seq_t r = tmp % (seq_t)11;
            if (r == (seq_t)1)      cnt1++;
            else if (r == (seq_t)10) cnt10++;
            tmp = q;
        }

        if (cnt1 == cnt10) {   /* val satisfies the property */
            counted++;
            if (counted == n)   /* found the n‑th term */
                return val;
        }

        val = val + (seq_t)1;  /* move to the next integer           */
    }

    /* The code never reaches this point */
    return val;
}
