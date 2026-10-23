/*
 * A119769  a(n) = (n-1)!*Sum_{k=1..n, gcd(k,n)=1} H(k), where H(k) =
 *          Sum_{j=1..k} 1/j, the k-th harmonic number.
 * https://oeis.org/A119769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{k=1..n, gcd(k,n)=1} H(k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where H(k) = Sum_{j=1..k} 1/j */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Special case n = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Compute (n-1)! as seq_t */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n - 1; ++i)
        fact *= (seq_t)i;

    /* Allocate a boolean array for coprime flags */
    size_t len = (size_t)n + 1;
    bool *coprime = (bool *)malloc(len * sizeof(*coprime));
    if (!coprime)
        return (seq_t)0;          /* unlikely, but guard */

    for (size_t i = 1; i <= n; ++i)
        coprime[i] = true;       /* initialise */

    /* Mark multiples of each prime factor of n as non‑coprime */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t p = 0; p < cnt; ++p) {
        uint64_t prime = primes[p];
        for (size_t k = prime; k <= n; k += prime)
            coprime[k] = false;
    }
    coprime[1] = true;            /* 1 is coprime to every n */

    /* Build prefix counts of coprime numbers up to i */
    idx_t *pref = (idx_t *)malloc((len) * sizeof(*pref));
    if (!pref) {
        free(coprime);
        return (seq_t)0;
    }
    pref[0] = 0;
    for (size_t i = 1; i <= n; ++i)
        pref[i] = pref[i - 1] + (coprime[i] ? 1 : 0);

    idx_t phi_n = pref[n];        /* number of k <= n coprime to n */

    /* Accumulate the sum (n-1)! * Sum_{j=1..n-1} s(j)/j  */
    seq_t sum = (seq_t)0;
    for (size_t j = 1; j <= (size_t)n - 1; ++j) {
        /* s(j) = #{ k : j <= k <= n, gcd(k,n)=1 } = phi_n - pref[j-1] */
        idx_t coprime_ge_j = phi_n - pref[j - 1];
        if (coprime_ge_j == 0)
            continue;              /* skip zero multiplier */

        seq_t term = (fact / (seq_t)j) * (seq_t)coprime_ge_j;
        sum += term;
    }

    free(coprime);
    free(pref);
    return sum;
}
