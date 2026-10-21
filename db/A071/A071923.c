/*
 * A071923  a(n) is the prime p such that pi(n^2, (n+1)^2+1) = pi((n+1)^2, p)
 *          where pi(s,t) = pi(t) - pi(s) is the number of primes between s
 *          and t.
 * https://oeis.org/A071923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime p > (n+1)^2 such that #primes in
 *            (n^2,(n+1)^2] equals #primes in ((n+1)^2,p]. This is achieved by
 *            counting the primes up to (n+1)^2, then scanning forward for the
 *            C‑th prime after (n+1)^2, where C = pi((n+1)^2) - pi(n^2)
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   compute_pi, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* Squares as seq_t to prevent overflow. */
    seq_t n_sq  = (seq_t)n * (seq_t)n;
    seq_t np_sq = (seq_t)(n + 1) * (seq_t)(n + 1);

    /* Count of primes in (n^2,(n+1)^2] */
    seq_t C = compute_pi((idx_t)np_sq) - compute_pi((idx_t)n_sq);

    seq_t count = 0;
    seq_t cand  = np_sq + 1;      /* first integer > (n+1)^2 */

    for (;; ++cand) {
        if (isPrimeGeneric(cand)) {
            ++count;
            if (count == C)
                return cand;
        }
    }
}
