/*
 * A195106  Numbers with largest and smallest prime factors differing by 4.
 * https://oeis.org/A195106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 85, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A195106: numbers whose largest and smallest prime
 * factors differ by 4.  The function scans integers, determining
 * for each its least‑prime factor (lpf) and greatest‑prime factor
 * (gpf) by means of a smallest‑prime‑factor sieve, and collects
 * those with gpf - lpf == 4.  It does not rely on a closed formula
 * and works correctly for any idx_t (uint64_t) and seq_t (fixed‑size
 * unsigned integer or arbitrary‑precision integer). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static void sieve_spf(uint32_t *spf, idx_t limit)
{
    /* Compute smallest prime factor for every number up to limit. */
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = (uint32_t)i;
    for (idx_t i = 2; (idx_t)i * (idx_t)i <= limit; ++i)
    {
        if (spf[i] != (uint32_t)i)
            continue;   /* not prime */
        for (idx_t j = i * i; j <= limit; j += i)
            if (spf[j] == (uint32_t)j)
                spf[j] = (uint32_t)i;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined for offset 1 */

    /* Initial limit for sieve; will double if necessary. */
    idx_t limit = 1024;
    idx_t last_checked = 1;   /* last number processed */
    idx_t count = 0;          /* how many qualifying numbers seen */
    seq_t result = (seq_t)0;  /* value to return */

    while (1)
    {
        /* Allocate and compute SPF up to current limit. */
        uint32_t *spf = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));
        if (!spf)
            return (seq_t)0;    /* memory failure */
        sieve_spf(spf, limit);

        /* Scan new numbers from last_checked+1 to limit. */
        for (idx_t i = last_checked + 1; i <= limit; ++i)
        {
            uint32_t lpf = spf[i];
            uint32_t gpf = lpf;   /* will be overwritten with largest factor */
            uint32_t temp = (uint32_t)i;
            while (temp > 1)
            {
                uint32_t p = spf[temp];
                gpf = p;
                while (temp % p == 0)
                    temp /= p;
            }

            if ((seq_t)gpf - (seq_t)lpf == (seq_t)4)
            {
                ++count;
                if (count == n)
                {
                    result = (seq_t)i;
                    free(spf);
                    return result;
                }
            }
        }

        /* Not enough terms found; increase limit and retry. */
        free(spf);
        last_checked = limit;
        limit <<= 1;          /* double the limit */
    }
}
