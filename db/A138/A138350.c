/*
 * A138350  Moment sequence of tr(A^2) in USp(4).
 * https://oeis.org/A138350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{q=0}^{⌊n/2⌋} (2m)!/(q!(q+1)!(m-q)!²) for n=2m -
 *            Σ_{q=0}^{⌊(n-1)/2⌋} (2m+1)!/(q!(q+1)!(m-q)!(m-q+1)!) for n=2m+1
 *            Recurrence: T_{q+1}=T_q·(m-q)²/((q+1)(q+2)), (even n)
 *            T'_{q+1}=T'_q·(m-q+1)(m-q)/((q+1)(q+2)) (odd n)
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 17, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute binomial coefficient C(n,k) exactly in seq_t. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t r = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
        r = (r * (seq_t)(n - k + i)) / (seq_t)i;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;

    if ((n & 1) == 0) {            /* even n = 2*m */
        idx_t m = n >> 1;
        seq_t term = binom((idx_t)2 * m, m);   /* T0 */
        seq_t sum  = term;

        for (idx_t q = 0; q < m; ++q) {
            idx_t rem = m - q;
            seq_t num = (seq_t)rem * (seq_t)rem;          /* (m-q)² */
            seq_t den = (seq_t)(q + 1) * (seq_t)(q + 2);   /* (q+1)(q+2) */
            term = (term * num) / den;                    /* exact */
            sum += term;
        }
        return sum;
    } else {                        /* odd n = 2*m+1 */
        idx_t m = (n - 1) >> 1;
        seq_t term = binom((idx_t)2 * m + 1, m);   /* T'0 */
        seq_t sum  = term;

        for (idx_t q = 0; q < m; ++q) {
            idx_t rem = m - q;
            seq_t num = (seq_t)(rem + 1) * (seq_t)rem;   /* (m-q+1)(m-q) */
            seq_t den = (seq_t)(q + 1) * (seq_t)(q + 2); /* (q+1)(q+2) */
            term = (term * num) / den;
            sum += term;
        }
        return -(sum);
    }
}
