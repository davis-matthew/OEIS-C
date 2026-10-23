/*
 * A102304  Sum of prime factors (counted with multiplicity) of the numbers
 *          having exactly three prime factors.
 * https://oeis.org/A102304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the prime factors (with multiplicity) of the n‑th
 *            integer with exactly three prime factors
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state shared across calls */
    static idx_t current_max = 0;             /* largest index that has been sieved */
    static uint32_t *spf = NULL;              /* smallest prime factor table */
    static uint8_t   *omega = NULL;           /* number of prime factors (with multiplicity) */
    static seq_t    *sumpf = NULL;            /* sum of prime factors */
    static seq_t    *results = NULL;          /* a(n) values for numbers with Omega=3 */
    static size_t results_cnt = 0;            /* number of values stored in results */

    if (n == 0) return (seq_t)0;   /* undefined, but guard against 0 */

    /* Ensure we have at least n results computed */
    if (results_cnt < (size_t)n)
    {
        size_t target = (size_t)n;
        size_t limit = current_max ? (size_t)current_max : 51200; /* initial bound */

        while (results_cnt < target)
        {
            /* free old tables */
            free(spf);
            free(omega);
            free(sumpf);
            free(results);

            /* allocate new tables */
            spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
            omega = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
            sumpf = (seq_t*)   calloc(limit + 1, sizeof(seq_t));

            if (!spf || !omega || !sumpf) return (seq_t)0; /* allocation failure */

            /* Simple sieve: smallest prime factor for each number */
            for (size_t i = 2; i <= limit; ++i)
            {
                if (spf[i] == 0)
                {
                    spf[i] = (uint32_t)i;
                    if (i <= limit / i)    /* avoid overflow of i*i     */
                        for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                            if (spf[(size_t)j] == 0)
                                spf[(size_t)j] = (uint32_t)i;
                }
            }

            /* Compute Omega and sum of prime factors, and collect numbers with Omega==3 */
            results_cnt = 0;
            /* First pass: count how many will be stored */
            for (size_t i = 2; i <= limit; ++i)
            {
                uint32_t p = spf[i];
                size_t quo = i / p;                /* i / smallest prime factor */
                omega[i] = omega[quo] + 1;
                sumpf[i] = sumpf[quo] + (seq_t)p;
                if (omega[i] == 3) results_cnt++;
            }

            /* Allocate results array */
            results = (seq_t*)calloc(results_cnt, sizeof(seq_t));
            if (!results && results_cnt != 0) return (seq_t)0;

            /* Second pass: store the sums in order */
            size_t idx = 0;
            for (size_t i = 2; i <= limit; ++i)
                if (omega[i] == 3)
                    results[idx++] = sumpf[i];

            current_max = limit;

            if (results_cnt >= target) break;   /* we have enough values */

            /* Not enough yet – extend the sieve range */
            limit = current_max * 2;
        }
    }

    return results[(size_t)n - 1];
}
