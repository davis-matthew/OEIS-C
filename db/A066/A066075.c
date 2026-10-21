/*
 * A066075  Number of solutions x to prime(n) = sigma(x) - 1, where prime(n)
 *          is the n-th prime.
 * https://oeis.org/A066075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066075: Number of solutions x to prime(n) = sigma(x) – 1.
   Formula: a(n) = A054973(prime(n)+1)
   The function precomputes σ(x) for all x ≤ prime(n) and counts how many
   x have a given σ‑value.  It is memoised so that successive calls with
   larger n are evaluated efficiently.  The routine uses only integer
   arithmetic and standard C11 headers.                                         */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* static data that persist across calls */
    static idx_t cached_n = 0;               /* largest n that has been processed */

    /* dynamically allocated arrays */
    static uint32_t *primes  = NULL;         /* list of primes up to current limit */
    static size_t   primes_cnt = 0;          /* number of primes stored */

    static uint32_t *spf     = NULL;         /* smallest prime factor for each i */
    static uint32_t *sigma    = NULL;         /* σ(i) for each i */

    static uint32_t *cnt      = NULL;         /* cnt[s] = number of i with σ(i)=s */
    static uint32_t  cnt_size = 0;            /* size of cnt array */



    /* ------------------------------------------------------------------
       Recompute everything if a larger n is requested.
     ------------------------------------------------------------------ */
    if (n > cached_n)
    {
        /*----------------------------
           1. Determine the limit needed for nth prime
          ----------------------------*/
        double dn = (double)n;
        double ub;
        if (n < 6)
            ub = 15.0;                         /* small n, 15 is sufficient */
        else
            ub = dn * (log(dn) + log(log(dn))) + 10.0;  /* Rosser–Schoenfeld + cushion */
        idx_t limit = (idx_t)ub;
        if (limit < 3) limit = 3;

        /* extend limit until we have enough primes */
        while (1)
        {
            /* allocate spf */
            free(spf);
            spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
            if (!spf) return (seq_t)0;          /* allocation failure */

            /* simple sieve computing spf */
            for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
            primes_cnt = 0;
            free(primes);
            primes = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
            if (!primes) return (seq_t)0;

            for (idx_t i = 2; i <= limit; ++i)
            {
                if (spf[i] == 0)
                {
                    spf[i] = (uint32_t)i;
                    primes[primes_cnt++] = (uint32_t)i;
                    if (i * i <= limit)
                        for (idx_t j = i * i; j <= limit; j += i)
                            if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }

            if (primes_cnt >= n) break;         /* enough primes */
            limit <<= 1;                         /* double limit and try again */
        }

        /*----------------------------
           2. Compute σ(i) for all i ≤ limit
          ----------------------------*/
        free(sigma);
        sigma = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        if (!sigma) return (seq_t)0;

        sigma[0] = 0;
        sigma[1] = 1;
        for (idx_t i = 2; i <= limit; ++i)
        {
            idx_t p   = spf[i];
            idx_t m   = i;
            uint64_t term = 1;          /* p^exp */
            idx_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
                term *= (uint64_t)p;
            }
            /* sum of powers: 1 + p + ... + p^exp = (p^(exp+1)-1)/(p-1) */
            uint64_t sumPow = (term * (uint64_t)p - 1ull) / (p - 1);
            sigma[i] = sigma[m] * (uint32_t)sumPow;
        }

        /*----------------------------
           3. Build count array up to maximum σ value seen
          ----------------------------*/
        uint32_t maxSigma = 0;
        for (idx_t i = 1; i <= limit; ++i)
            if (sigma[i] > maxSigma) maxSigma = sigma[i];

        free(cnt);
        cnt = (uint32_t *)calloc(maxSigma + 1, sizeof(uint32_t));
        if (!cnt) return (seq_t)0;

        for (idx_t i = 1; i <= limit; ++i)
            cnt[sigma[i]]++;

        cnt_size = maxSigma + 1;

        /* update cache marker */
        cached_n = n;
    }

    /* ------------------------------------------------------------------
       4. Return the result for the requested n
      ------------------------------------------------------------------*/
    uint32_t prime_val = primes[n - 1];          /* nth prime (1‑based) */
    uint32_t target   = prime_val + 1;           /* we count σ(x) == prime+1 */

    if (target < cnt_size)
        return (seq_t)cnt[target];
    else
        return (seq_t)0;                         /* should not happen for n≥1 */
}
