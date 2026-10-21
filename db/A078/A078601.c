/*
 * A078601  Number of ways to lace a shoe that has n pairs of eyelets,
 *          assuming the lacing satisfies certain conditions.
 * https://oeis.org/A078601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n!)^2/2)*Sum_{k=0..floor(n/2)} binomial(n-k,k)^2/(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 1;

    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) fact *= (seq_t)i;

    seq_t coeff = fact * fact;
    coeff /= 2;

    idx_t maxk = n >> 1;          /* floor(n/2) */
    seq_t fdiv = coeff / (seq_t)n; /* coeff/(n-k) for current k */
    seq_t bin   = 1;               /* C(n,0) */
    seq_t sum   = 0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        sum += fdiv * bin * bin;

        if (k == maxk) break;

        /* Prepare for the next iteration (k -> k+1) */

        /* Update denominator: from (n-k) to (n-k-1) */
        seq_t m = (seq_t)(n - k);
        fdiv = fdiv * m / (m - 1);

        /* Update bin: from C(n-k, k) to C(n-(k+1), k+1) */
        idx_t nextk = k + 1;
        seq_t num = (seq_t)(n - 2 * k);
        num *= (seq_t)(n - 2 * k - 1);
        seq_t den = (seq_t)nextk * (seq_t)(n - k);
        bin = bin * num / den;
    }

    return sum;
}
