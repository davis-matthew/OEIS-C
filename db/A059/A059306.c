/*
 * A059306  Number of 2 X 2 singular integer matrices with elements from
 *          {0,...,n}.
 * https://oeis.org/A059306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (n+1)^2 + Σ_{m=1..n} (2*φ(m) + (m==1?1:0)) * ⌊n/m⌋ *
 *            (⌊n/m⌋ + 1) where φ(m) is Euler's totient function. The factor
 *            2*φ(m) counts ordered coprime pairs (a,b) with max(a,b)=m (m≥2).
 *            For m=1, there is one extra pair (1,1) giving (m==1?1:0)
 * Range:     u8 n <= 6, u16 n <= 85, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate array of φ values */
    idx_t *phi = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!phi)
        return (seq_t)0;          /* allocation failure, should never happen */

    /* initialize φ[i] = i */
    for (idx_t i = 0; i <= n; ++i)
        phi[i] = i;

    /* linear sieve for φ */
    for (idx_t i = 2; i <= n; ++i) {
        if (phi[i] == i) {                      /* i is prime */
            for (idx_t j = i; j <= n; j += i)
                phi[j] -= phi[j] / i;
        }
    }

    /* base term (n+1)^2 */
    seq_t total = (seq_t)(n + 1) * (n + 1);

    /* accumulate contributions for each m */
    for (idx_t m = 1; m <= n; ++m) {
        idx_t count_pairs = (m == 1 ? 3 : 2 * phi[m]); /* ordered coprime pairs with max=m */
        idx_t T = n / m;                               /* floor(n/m) */
        seq_t term = (seq_t)T * (T + 1);               /* T*(T+1) */
        total += (seq_t)count_pairs * term;
    }

    free(phi);
    return total;
}
