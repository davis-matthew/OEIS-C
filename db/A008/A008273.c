/*
 * A008273  Number of performances of n fragments in Stockhausen problem.
 * https://oeis.org/A008273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n, k) * ((2*k)! / 2^k - k * k!)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* not used, offset is 1 */

    seq_t total = 0;
    seq_t fk = 1;          /* k! */
    seq_t term1 = 1;       /* (2k)! / 2^k = ∏_{j=1..k} (2j-1) * j */

    for (idx_t k = 1; k <= n; ++k) {
        fk *= (seq_t)k;                 /* fk = k!              */
        term1 *= (seq_t)(2 * k - 1) * (seq_t)k;   /* term1 = (2k)!/2^k  */

        seq_t sub = term1 - fk * (seq_t)k;        /* term1 - k·k!   */
        seq_t bin = binomial(n, k);               /* C(n,k)         */
        total += bin * sub;                       /* accumulate     */
    }

    return total;
}
