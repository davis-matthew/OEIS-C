/*
 * A228485  Odd prime powers p^k such that p is congruent to 2 or 5 mod 9.
 * https://oeis.org/A228485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth odd prime power p^k with p congruent to 2 or 5
 *            modulo 9. Procedure: use a dynamic upper bound and generate all
 *            suitable prime powers up to that bound, then sort them. Continue
 *            doubling the bound until at least n terms have been produced.
 *            This works for any seq_t (built‑in unsigned...
 * Range:     u8 none, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto()   */

/* comparison function for qsort when seq_t may be a class */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* External linkage as requested */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, guard anyway */

    /* Start with a reasonable upper bound for the nth term. */
    idx_t limit = (n < 1000) ? 1000 : n * 10;
    /* Ensure limit is at least 10 to avoid very small searches. */
    if (limit < 10) limit = 10;

    for (;;)
    {
        /* Generate all primes up to *limit* (inclusive). */
        uint64_t *primes = NULL;
        size_t prime_cnt = 0;
        build_primes_upto(limit, &primes, &prime_cnt);
        if (!primes) return 0;  /* allocation failure */

        /* Buffer for prime powers. */
        size_t vals_cap = (prime_cnt * 4) + 16;
        seq_t *vals = (seq_t *)malloc(vals_cap * sizeof(seq_t));
        if (!vals) { free(primes); return 0; }

        size_t vals_len = 0;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t p = primes[i];
            if (p <= 2) continue;                       /* skip even prime 2   */
            if (p % 9 != 2 && p % 9 != 5) continue;     /* required congruence */

            seq_t val = (seq_t)p;
            /* Generate successive powers of p until > limit. */
            while (val <= (seq_t)limit)
            {
                if (vals_len == vals_cap)
                {
                    vals_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(vals,
                                                  vals_cap * sizeof(seq_t));
                    if (!tmp) { free(vals); free(primes); return 0; }
                    vals = tmp;
                }
                vals[vals_len++] = val;

                /* Prevent overflow of val * p and keep within the bound. */
                if (val > (seq_t)limit / (seq_t)p) break;
                val *= (seq_t)p;
            }
        }

        free(primes);

        /* Sort the collected values to obtain order of the sequence. */
        qsort(vals, vals_len, sizeof(seq_t), cmp_seq);

        if (vals_len >= n)
        {
            seq_t result = vals[n - 1];   /* zero‑based index */
            free(vals);
            return result;
        }

        /* Not enough terms: free data and double the search limit. */
        free(vals);
        limit *= 2;
    }
}
