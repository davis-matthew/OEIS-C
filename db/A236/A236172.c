/*
 * A236172  a(n) = ( a(n-2)*(3*a(n-3) + a(n-2)) + a(n-1)*(a(n-1) + 2*a(n-2) +
 *          a(n-3)) ) / ( 2*a(n-3) - a(n-2) - 3*a(n-1) ), with a(0)=-3,
 *          a(1)=1, a(2)=-2, a(3)=2.
 * https://oeis.org/A236172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ( a(n-2)*(3*a(n-3) + a(n-2)) + a(n-1)*(a(n-1) + 2*a(n-2)
 *            + a(n-3)) ) / ( 2*a(n-3) - a(n-2) - 3*a(n-1) ), with a(0)=-3,
 *            a(1)=1, a(2)=-2, a(3)=2 a(n) is periodic with period 30 for all
 *            n ≥ 0
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial four terms directly. */
    if (n == 0u) return -3;
    if (n == 1u) return 1;
    if (n == 2u) return -2;
    if (n == 3u) return 2;

    /* For n ≥ 4, compute iteratively using the recurrence. */
    seq_t a_n_minus_3 = 1;   /* a(1)  */
    seq_t a_n_minus_2 = -2;  /* a(2)  */
    seq_t a_n_minus_1 = 2;   /* a(3)  */
    seq_t a_k = 0;           /* placeholder for a(n) */

    for (idx_t k = 4; k <= n; ++k) {
        seq_t num = a_n_minus_2 * (3 * a_n_minus_3 + a_n_minus_2)
                  + a_n_minus_1 * (a_n_minus_1 + 2 * a_n_minus_2 + a_n_minus_3);
        seq_t den = 2 * a_n_minus_3 - a_n_minus_2 - 3 * a_n_minus_1;
        a_k = num / den;  /* integer division, denominator never zero */

        /* Shift the window for the next iteration. */
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_k;
    }

    return a_k;
}
