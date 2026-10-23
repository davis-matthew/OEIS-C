/*
 * A049395  Expansion of (1-25*x)^(-7/5).
 * https://oeis.org/A049395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^n/n!) * ∏_{k=0}^{n-1} (5*k+7)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Equivalent recurrence a(0)=1, a(n)=((25*n+10)/n)*a(n-1) */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t res = 1;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_seq   = (seq_t)k;
        seq_t numer   = (seq_t)25 * (seq_t)k + (seq_t)10; /* 25*k + 10 */

        /* Reduce res and the denominator k before multiplication to avoid overflow. */
        seq_t g       = seq_gcd(res, k_seq);   /* gcd(res, k)   */
        seq_t res1    = res / g;               /* res / gcd     */
        seq_t k1      = k_seq / g;             /* k   / gcd     */
        seq_t numer_div = numer / k1;          /* numerator / k1, k1 | numer */

        res = res1 * numer_div;                /* new a(k)       */
    }

    return res;
}
