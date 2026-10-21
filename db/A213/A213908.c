/*
 * A213908  Minimal number of terms in the series 1/n + 1/(n+1) + 1/(n+2) +
 *          ... to obtain a sum >= 1.
 * https://oeis.org/A213908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k such that Σ_{i=n}^{n+k-1} 1/i ≥ 1
 * Range:     u8 n <= 148, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the minimal number of consecutive terms in the harmonic
   series starting at 1/n whose sum is at least 1.  The function
   performs a simple incremental sum using double precision to avoid
   costly rational arithmetic, which is sufficient for the required
   exactness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used for this sequence */
    double sum = 0.0;
    idx_t i = n;
    seq_t count = 0;

    for (;;) {
        sum += 1.0 / (double)i;
        ++count;
        if (sum >= 1.0) {
            return count;
        }
        ++i;
    }
}
