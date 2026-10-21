/*
 * A078503  a(n) = binomial(phi(n+1),phi(n)).
 * https://oeis.org/A078503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(phi(n+1),phi(n))
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 35, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   binomial, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* behaviour below the offset is irrelevant */
        return 0;

    seq_t phi_n    = totient(n);
    seq_t phi_np1  = totient(n + 1);

    /* The binomial routine expects idx_t arguments.  φ(n) ≤ n, so the cast
       is safe for all n that fit into idx_t. */
    idx_t N = (idx_t)phi_np1;
    idx_t K = (idx_t)phi_n;

    /* When K > N the binomial coefficient is zero. */
    if (K > N)
        return 0;

    /* Compute binomial(N,K) using the supplied utilities. */
    return binomial(N, K);
}
