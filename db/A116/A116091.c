/*
 * A116091  Expansion of 1/sqrt(1+4*x+16*x^2).
 * https://oeis.org/A116091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} binom(n,k)^2 * (-3)^k (recursively
 *            equivalent to the given D-finite recurrence)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 17, i64 n <= 32, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Base case */
    if (n == 0) return (seq_t)1;

    seq_t sum = 1;          /* k = 0 term: C(n,0)^2 * (-3)^0 = 1 */
    seq_t c   = 1;          /* current binomial C(n,k) */
    seq_t pow3 = 1;         /* (-3)^k */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial coefficient: C(n,k) = C(n,k-1) * (n-k+1) / k */
        seq_t mult_factor = (seq_t)(n - k + 1);
        c = c * mult_factor;
        c = c / k;                      /* exact division */

        /* Update (-3)^k */
        pow3 = pow3 * (seq_t)(-3);

        /* term = C(n,k)^2 * (-3)^k */
        seq_t term = c * c;
        term = term * pow3;

        sum += term;
    }
    return sum;
}
