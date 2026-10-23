/*
 * A147581  Numbers with exactly 8 distinct odd prime divisors
 *          {3,5,7,11,13,17,19,23}.
 * https://oeis.org/A147581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 111546435 * v(n) where v(n) is the n-th * positive
 *            integer whose prime factors belong to {3,5,7,11,13,17,19,23}. *
 *            (v(1)=1, v(2)=3, v(3)=5, v(4)=7, v(5)=9, …)
 * Range:     u8 none, u16 none, u32 n <= 16, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* At least one term is required. */
    if (n == 0) return (seq_t)0;        /* undefined for n=0, safe guard */

    /* primes involved (all odd). */
    static const idx_t primes[8] = {3,5,7,11,13,17,19,23};

    /* Allocate array to hold the first n members of the multiplicative semigroup. */
    seq_t *t = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!t) return (seq_t)0;            /* allocation failed */

    /* indices for the multi‑pointer method (the ugly‑number algorithm). */
    size_t idx[8] = {0,0,0,0,0,0,0,0};

    t[0] = (seq_t)1;                    /* first element of the semigroup */

    for (idx_t pos = 1; pos < n; ++pos) {
        /* compute next candidate */
        seq_t min_val = (seq_t)primes[0] * t[idx[0]];
        for (size_t i = 1; i < 8; ++i) {
            seq_t cand = (seq_t)primes[i] * t[idx[i]];
            if (cand < min_val) min_val = cand;
        }
        t[pos] = min_val;

        /* advance all pointers that produced this min_val */
        for (size_t i = 0; i < 8; ++i) {
            if ((seq_t)primes[i] * t[idx[i]] == min_val)
                ++idx[i];
        }
    }

    /* Multiply by the fixed product of all eight primes. */
    const seq_t product = (seq_t)111546435ULL;   /* 3*5*7*11*13*17*19*23 */
    seq_t result = product * t[n-1];

    free(t);
    return result;
}
