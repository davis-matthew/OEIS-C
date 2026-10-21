/*
 * A066294  a(n) = A000203(n)^2 - A001157(n) - 2n = sigma(n)^2 - sigma_2(n) -
 *          2n.
 * https://oeis.org/A066294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(n)^2 - sigma_2(n) - 2*n
 * Range:     i8 n <= 9, i16 n <= 83, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A066294.  The function is
   independent of external prime tables and works for any n ≥ 1.
   It runs in O(√n) time, which is well within the required limits
   for n up to about 10 000. */
seq_t a(idx_t n)
{
    /* For the specified offset: n ≥ 1.  For n == 0 we return 0,
       though this case is outside the sequence. */
    if (n == 0)
        return (seq_t)0;

    seq_t sigma   = 0;   /* σ(n)  */
    seq_t sigma2  = 0;   /* σ₂(n) */

    /* Scan divisors d from 1 up to floor(sqrt(n)). */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t sd  = (seq_t)d;
            seq_t d2  = sd * sd;
            sigma  += sd;
            sigma2 += d2;

            idx_t comp = n / d;
            if (comp != d) {
                seq_t sc   = (seq_t)comp;
                seq_t c2   = sc * sc;
                sigma  += sc;
                sigma2 += c2;
            }
        }
    }

    /* a(n) = σ(n)^2 - σ₂(n) - 2n */
    seq_t n_seq  = (seq_t)n;
    seq_t result = sigma * sigma - sigma2 - 2 * n_seq;
    return result;
}
