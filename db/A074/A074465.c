/*
 * A074465  a(n) = gcd(n^2, sigma(n^2), phi(n^2)).
 * https://oeis.org/A074465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n^2, sigma(n^2), phi(n^2))
 * Range:     u8 n <= 11, u16 n <= 1931, u32 n <= 9902, u64 n <= 9902,
 *            u128 n <= 9902, big any n
 * Helpers:   gcd, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* For n >= 1 (offset 1). Return 0 when n==0 for safety. */
    if (n == 0) return 0;

    idx_t sq = (idx_t)n * (idx_t)n;   /* n^2 as an idx_t value */
    seq_t sqv = (seq_t)sq;            /* n^2 as seq_t for the gcd */

    seq_t sig = sigma_of(sq);         /* sigma(n^2) */
    seq_t ph  = phi(sq);              /* phi(n^2) */

    seq_t g = gcd(gcd(sqv, sig), ph); /* gcd of the three values */

    return g;
}
