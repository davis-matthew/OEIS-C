/*
 * A227758  a(n) = sigma(sigma(n)) - sigma(n) - n, where sigma(n) = A000203(n)
 *          = sum of the divisors of n.
 * https://oeis.org/A227758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sigma(sigma(n)) - sigma(n) - n
 * Range:     i8 n <= 53, i16 n <= 4319, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9592 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of the divisors of a value n given as seq_t.
   It works for any seq_t type that supports the usual arithmetic
   operations and division/modulo with a seq_t. */
static seq_t sigma_generic(seq_t n)
{
    if (n == 0) return 0;          /* not used, but safe */
    seq_t sum = 0;
    /* Find floor(sqrt(n)) by incrementally testing squares. */
    idx_t lim = 0;
    while (((seq_t)(lim + 1) * (seq_t)(lim + 1)) <= n) ++lim;
    for (idx_t i = 1; i <= lim; ++i) {
        seq_t i_seq = (seq_t)i;
        if (n % i_seq == 0) {
            sum += i_seq;
            seq_t other = n / i_seq;
            if (other != i_seq) sum += other;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t s      = sigma_generic(n_seq);   /* sigma(n)           */
    seq_t ss     = sigma_generic(s);       /* sigma(sigma(n))    */
    return ss - s - n_seq;                 /* a(n)               */
}
