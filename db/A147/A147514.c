/*
 * A147514  Least number m, written in base 10, such that m/2 is obtained
 *          merely by shifting the leftmost digit of m to the right end, and
 *          2m by shifting the rightmost digit of m to the left end, digits
 *          defined in base n.
 * https://oeis.org/A147514
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  base, nonn
 * Formula:   Computes the least number m in base n such that * m/2 is
 *            obtained by shifting the leftmost digit of m to the right end *
 *            and 2*m is obtained by shifting the rightmost digit of m to the
 *            left end. * * The function follows the algorithm from the OEIS
 *            entry: * m*(2b-1) = 2*d_e*(b^(k+2)-1) where * b =...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* undefined for n < 3 */

    idx_t b = n;                  /* base */
    seq_t best = 0;               /* minimal m found */

    const seq_t denom = (seq_t)(2*b - 1);   /* 2b-1 */

    seq_t pow_b_k = 1;            /* b^k, start with k=0 */

    for (idx_t k = 0; ; ++k)
    {
        /* Compute b^(k+1) and b^(k+2) incrementally */
        seq_t pow_b_kplus1 = pow_b_k * (seq_t)b;          /* b^(k+1) */
        seq_t pow_b_kplus2 = pow_b_kplus1 * (seq_t)b;     /* b^(k+2) */

        /* Lower bound on the smallest possible m for this k */
        seq_t min_candidate = ((seq_t)2 * (pow_b_kplus2 - 1)) / denom;
        if (best != 0 && min_candidate >= best) {
            break;  /* further k will only produce larger m */
        }

        /* Iterate over possible most-significant digits */
        for (idx_t de = 1; de < b; ++de)
        {
            seq_t rhs = ((seq_t)2 * de) * (pow_b_kplus2 - 1);
            if (rhs % denom != 0) continue;

            seq_t m = rhs / denom;
            if (best != 0 && m >= best) continue;
            if (m % 2 != 0) continue;            /* m must be even */

            /* Extract least significant digit d0 */
            seq_t d0 = m % (seq_t)b;

            /* Compute middle digits d_m */
            seq_t tmp = m - d0 - ( (seq_t)de * pow_b_kplus1 );
            if (tmp < 0 || tmp % (seq_t)b != 0) continue;
            seq_t d_m = tmp / (seq_t)b;

            /* d_m must fit in k digits */
            if (d_m >= pow_b_k) continue;

            /* Verify rotation conditions */
            seq_t expected1 = d_m * ( (seq_t)b * (seq_t)b ) + d0 * (seq_t)b + (seq_t)de;
            if (m / 2 != expected1) continue;

            seq_t expected2 = d0 * pow_b_kplus1 + (seq_t)de * pow_b_k + d_m;
            if (m * 2 != expected2) continue;

            /* Found a valid candidate */
            best = m;
        }

        pow_b_k = pow_b_kplus1;   /* advance k */
    }

    return best;
}
