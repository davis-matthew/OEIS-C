/*
 * A181933  a(n) = Sum_{k=0..n} binomial(n+k,k)*sin(Pi*(n+k)/2).
 * https://oeis.org/A181933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k,k) * sin(Pi*(n+k)/2)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 17, i64 n <= 33, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for
 *   a(n) = Σ_{k=0}^{n} binomial(n+k,k) * sin(π(n+k)/2)          */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        idx_t m = n + k;                /* m = n + k */
        if ((m & 1) == 0)               /* sin = 0 for even m */
            continue;

        seq_t term;

        /* Calculate C(m,k) where m = n+k and k ≤ n.  Use a larger
         * intermediate type when seq_t is a small integer type to
         * avoid overflow.  For large seq_t (e.g. arbitrary‑precision)
         * we use seq_t directly. */
        if (sizeof(seq_t) <= 8) {
            __int128 t = 1;
            for (idx_t i = 1; i <= k; ++i) {
                t = t * (m - k + i);
                t /= i;
            }
            term = (seq_t)t;
        } else {
            term = 1;
            for (idx_t i = 1; i <= k; ++i) {
                term = term * (seq_t)(m - k + i);
                term = term / (seq_t)i;
            }
        }

        /* sin(πm/2) =  1 for m ≡ 1 (mod 4) and -1 for m ≡ 3 (mod 4) */
        if ((m & 3) == 1)
            sum += term;
        else
            sum -= term;
    }

    return sum;
}
