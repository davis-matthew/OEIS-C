/*
 * A043822  Numbers k such that number of runs in the base 3 representation of
 *          k is congruent to 7 mod 10.
 * https://oeis.org/A043822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k such that the number of runs in the
 *            base‑3 representation of k is congruent to 7 (mod 10)
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k = 1;                 /* current candidate integer          */
    idx_t found = 0;             /* how many suitable numbers seen    */

    while (found < n) {
        idx_t t = k;
        int prev_digit = -1;     /* previous base‑3 digit, none yet   */
        idx_t runs = 0;          /* number of runs found              */

        /* Count runs in base‑3 representation of k. */
        while (t > 0) {
            int digit = t % 3;   /* current least significant digit    */
            if (digit != prev_digit) {
                runs++;
                prev_digit = digit;
            }
            t /= 3;
        }

        /* Check the required congruence. */
        if (runs % 10 == 7) {
            found++;
            if (found == n)
                return (seq_t)k;
        }
        k++;                      /* next integer to test             */
    }

    /* In theory the loop always returns before reaching this point. */
    return (seq_t)0;
}
