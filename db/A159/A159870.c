/*
 * A159870  Numerator of Hermite(n, 6/23).
 * https://oeis.org/A159870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 12*a(n-1) - 1058*(n-1)*a(n-2), a(0)=1, a(1)=12
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A159870: numerator of Hermite(n,6/23). */
seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 0) return 1;
    if (n == 1) return 12;

    /* a0 = 1, a1 = 12. */
    seq_t a_prev2 = 1;   /* a(n-2) */
    seq_t a_prev1 = 12;  /* a(n-1) */
    seq_t a_curr;

    /* Iterate from 2 up to n, applying the recurrence */
    for (idx_t k = 2; k <= n; ++k) {
        /* a(k) = 12*a(k-1) - 1058*(k-1)*a(k-2) */
        seq_t term1 = 12 * a_prev1;
        seq_t coeff = 1058 * (seq_t)(k - 1);
        seq_t term2 = coeff * a_prev2;
        a_curr = term1 - term2;

        /* Shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
