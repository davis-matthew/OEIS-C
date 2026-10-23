/*
 * A039183  Numbers whose base-11 representation has the same number of 3's
 *          and 5's.
 * https://oeis.org/A039183
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

/* Sequence: a(n) = the n-th nonnegative integer whose base‑11 representation
 * contains the same number of 3’s and 5’s.  Offset: 1  (a(1)=0) */

seq_t a(idx_t n) {
    /* Iterate through all nonnegative integers, checking the digit condition,
     * and return the n‑th one found. */
    if (n == 0) {          /* Not used, but keeps the function well‑defined */
        return (seq_t)0;
    }

    seq_t value = 0;       /* current integer being tested */
    idx_t  found = 0;      /* how many valid numbers have been seen so far */

    for (;;) {
        /* Count 3's and 5's in the base‑11 representation of 'value'. */
        seq_t temp = value;
        idx_t count3 = 0, count5 = 0;
        while (temp > 0) {
            seq_t digit = temp % 11;
            if (digit == 3)  count3++;
            else if (digit == 5) count5++;
            temp /= 11;
        }
        if (count3 == count5) {
            found++;
            if (found == n) {
                return value;
            }
        }
        value++;
    }
}
