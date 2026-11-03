/*
 * A098511  Diagonal sums of a matrix associated to the inverse of a Catalan
 *          scaled binomial matrix.
 * https://oeis.org/A098511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..floor(n/2)}
 *            denominator((n-k+1)*binomial(n-k,k)/binomial(2*(n-k),n-k))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

/* Return the nth term of OEIS sequence A098511 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n / 2;
    for (idx_t k = 0; k <= max_k; ++k)
    {
        idx_t N = n - k;
        seq_t bin2N = binomial(2 * N, N);          /* C(2N,N)   */
        seq_t binNK = binomial(N, k);              /* C(N,k)    */
        seq_t num = (seq_t)(N + 1) * binNK;         /* (N+1)*C(N,k) */
        seq_t d   = gcd(bin2N, num);           /* gcd(C(2N,N),num) */
        seq_t den = bin2N / d;                     /* denominator of the fraction   */
        sum += den;
    }
    return sum;
}
