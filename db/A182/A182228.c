/*
 * A182228  a(n) = 3*a(n-2) - a(n-1) for n > 1, a(0) = 0, a(1) = 1.
 * https://oeis.org/A182228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-2) - a(n-1), a(0) = 0, a(1) = 1
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 27, i64 n <= 53, i128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }

    /* Use two variables to store the two most recent terms.
       The recurrence only needs a(n-2) and a(n-1). */
    seq_t a_prev2 = 0;          /* a(k-2) */
    seq_t a_prev1 = 1;          /* a(k-1) */

    /* Iterate up to n. */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t a_k = 3 * a_prev2 - a_prev1;  /* a(k) = 3*a(k-2) - a(k-1) */

        /* Shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_k;
    }

    /* a_prev1 now holds a(n) */
    return a_prev1;
}
