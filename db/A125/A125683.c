/*
 * A125683  Numerator of Sum_{k=1..n} (-1)^(k+1) * 1/(k*(k+1)).
 * https://oeis.org/A125683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator of sum_{k=1..n} (-1)^{k+1} * 1/(k*(k+1))
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 95,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, but return 0 for safety */

    seq_t p = 0;   /* current numerator (positive) */
    seq_t q = 1;   /* current denominator (positive) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* sign of the k-th term: +1 for odd k, -1 for even k */
        int sign_positive = (k & 1);

        /* term denominator: k*(k+1) as seq_t */
        seq_t dk = (seq_t)k * ((seq_t)k + 1);

        /* reduce addition of p/q + sign_positive/(dk) */
        seq_t g1 = gcd(q, dk);
        seq_t q2 = q / g1;          /* q/g1 */
        seq_t d2 = dk / g1;         /* dk/g1 */

        seq_t new_den = q * d2;     /* lcm of q and dk */

        seq_t new_num;
        if (sign_positive) {
            new_num = p * d2 + q2;  /* p/q + 1/dk */
        } else {
            new_num = p * d2 - q2;  /* p/q - 1/dk */
            /* by construction new_num remains non‑negative for all n≥1 */
        }

        /* make the fraction reduced */
        seq_t g2 = gcd(new_num, new_den);
        p = new_num / g2;
        q = new_den / g2;
    }

    return p;   /* numerator of the reduced fraction */
}
