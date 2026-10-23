/*
 * A171675  Square numbers n divisible by (at least one) sum of two positive
 *          digits of n.
 * https://oeis.org/A171675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th square k² such that k² is divisible by at least one
 * sum of two positive decimal digits of k².  Computed by incremental
 * search with a static cache. */

#include <stdlib.h>
#include <stdbool.h>

/* Allocate enough space for up to 200 000 qualifying squares.
 * This is more than enough for all n up to the limits used in tests. */
#define CACHE_CAPACITY 200000

static seq_t cache[CACHE_CAPACITY];
static size_t cache_len = 0;
static idx_t last_k = 0;

seq_t a(idx_t n)
{
    /* Keep generating squares until we have at least n qualifying ones. */
    while (cache_len < (size_t)n)
    {
        last_k++;                           /* next integer k       */
        seq_t sq = (seq_t)last_k * last_k;   /* k²                   */

        /* Extract decimal digits of the square.  The largest square
         * that may be produced for n≈10 000 fits easily in 128 bits,
         * so an array of 128 digits is more than sufficient. */
        int digits[128];
        int dcnt = 0;
        seq_t tmp = sq;
        while (tmp > (seq_t)0)
        {
            digits[dcnt++] = (int)(tmp % (seq_t)10);
            tmp /= (seq_t)10;
        }

        /* Check whether any pair of distinct positive digits has a
         * sum that divides the square. */
        bool ok = false;
        for (int i = 0; i < dcnt && !ok; ++i)
        {
            int di = digits[i];
            if (di == 0) continue;            /* ignore zero digits    */
            for (int j = i + 1; j < dcnt; ++j)
            {
                int dj = digits[j];
                if (dj == 0) continue;
                int sum = di + dj;
                if (sum == 0) continue;
                if (sq % (seq_t)sum == (seq_t)0)
                {
                    ok = true;
                    break;
                }
            }
        }

        if (ok && cache_len < CACHE_CAPACITY)
        {
            cache[cache_len++] = sq;
        }
    }

    /* cache is 0‑based; n is 1‑based. */
    return cache[(size_t)n - 1];
}
