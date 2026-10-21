/*
 * A061071  Number of distinct values in the list of number of divisors, d(j),
 *          j=1...n.
 * https://oeis.org/A061071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values among d(j) for j = 1 … n where
 *            d(j) is the number of positive divisors of j
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n >= 1 is the only useful range; for n < 1 we return 0 */
    if (n < (idx_t)1) return (seq_t)0;

    /* ------------------------------------------------------------------
       1. Compute least prime factor for every number up to n.
       2. From that derive tau[j] = number of divisors of j.
       3. Count distinct tau values.  ------------------------------------*/

    /* --- Least‑prime‑factor array ------------------------------------- */
    uint64_t *spf = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!spf) return (seq_t)0;                       /* allocation failure */
    spf[0] = 0;
    spf[1] = 1;                                     /* not used */

    for (size_t i = 2; i <= (size_t)n; ++i) spf[i] = 0;

    for (uint64_t i = 2; i <= (uint64_t)n; ++i) {
        if (spf[i] == 0) {                           /* i is prime */
            spf[i] = i;
            if (i * i <= (uint64_t)n) {
                for (uint64_t j = i * i; j <= (uint64_t)n; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* --- Tau array (divisor counts) ----------------------------------- */
    seq_t *tau = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!tau) { free(spf); return (seq_t)0; }

    tau[1] = (seq_t)1;

    for (uint64_t i = 2; i <= (uint64_t)n; ++i) {
        uint64_t p   = spf[i];
        uint64_t m   = i / p;
        uint64_t exp = 1;
        while (m % p == 0) { m /= p; ++exp; }

        tau[i] = tau[m];
        tau[i] *= (seq_t)(exp + 1);
    }

    /* --- Find maximum tau value --------------------------------------- */
    seq_t max_tau = (seq_t)0;
    for (uint64_t i = 1; i <= (uint64_t)n; ++i)
        if (tau[i] > max_tau) max_tau = tau[i];

    /* Cast to a size_t.  For the divisor‑count function the maximum
       value is far below SIZE_MAX for all n of interest. */
    size_t max_tau_sz = (size_t)max_tau;

    /* --- Count distinct tau values ----------------------------------- */
    bool *seen = (bool *)calloc(max_tau_sz + 1, sizeof(bool));
    if (!seen) { free(tau); free(spf); return (seq_t)0; }

    seq_t distinct = (seq_t)0;
    for (uint64_t i = 1; i <= (uint64_t)n; ++i) {
        size_t idx = (size_t)tau[i];
        if (!seen[idx]) {
            seen[idx] = true;
            ++distinct;
        }
    }

    free(tau);
    free(spf);
    free(seen);

    return distinct;
}
