/*
 * A127705  a(n) = mu(n) + Sum_{k|n, k>1} (k+1)*mu(n/k), where mu = A008683.
 * https://oeis.org/A127705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n)-mu(n) for n>1; a(1)=1
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mu_of, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/mu_of.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    seq_t ph = phi(n);
    int mu = mu_of(n);          /* mu∈{-1,0,1} */
    if (mu >= 0)
        return ph - (seq_t)mu;  /* phi(n) - mu(n)  */
    else
        return ph + (seq_t)(-mu);
}
