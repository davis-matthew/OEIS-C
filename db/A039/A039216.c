/*
 * A039216  Numbers whose base-12 representation has the same number of 0's
 *          and 7's.
 * https://oeis.org/A039216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 186, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Term: Numbers whose base-12 representation has the same number of
   0's and 7's.
   Computation: Naïve enumeration of integers; for each integer count
   zeros and sevens in base‑12.  The n-th term is the n-th integer
   (starting at 1) that satisfies zeros==sevens.  This meets the
   requirements for n up to about 10 000 and works for all
   seq_t/idx_t sizes. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)             /* offset is 1, but guard against 0 */
        return (seq_t)0;

    idx_t count = 0;
    idx_t val   = 1;               /* candidate integer */

    while (1) {                    /* scan integers until nth matches */
        unsigned zeros  = 0;
        unsigned sevens = 0;

        idx_t temp = val;
        /* Count zeros and sevens in base‑12 representation of val */
        while (temp > (idx_t)0) {
            unsigned digit = (unsigned)(temp % (idx_t)12);
            if (digit == 0)       zeros++;
            else if (digit == 7)  sevens++;
            temp /= (idx_t)12;
        }

        if (zeros == sevens) {
            count++;
            if (count == n)       return (seq_t)val;
        }
        val++;
    }
}
