/*
 * A076919  a(1) = 1, a(2) = 2, then a(n+1) is the smallest number such that
 *          the highest common factor of a(n) and a(n+1) is different from
 *          that of a(n) and a(n-1) and is more than 1.
 * https://oeis.org/A076919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2; for n>=3,
   a(n) is the smallest m > a(n-1) such that
   gcd(a(n-1),m) > 1 and gcd(a(n-1),m) != gcd(a(n-2),a(n-1)) */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

#define MAX_CACHE 10000 /* enough for all required n (<=10000) */

static seq_t cache[MAX_CACHE];
static idx_t cache_len = 0;   /* number of terms stored (0 means none computed yet) */

/* compute terms up to index n (1‑based) */
static void ensure_terms(idx_t n)
{
    if (n <= 0) return;
    if (cache_len >= n) return;              /* already done */

    /* initialise first two terms if needed */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = 2;
        if (n <= 2) return;
    }

    /* generate remaining terms */
    while (cache_len < n) {
        seq_t prev = cache[cache_len - 1];          /* a(i-1)   */
        seq_t prevprev = cache[cache_len - 2];      /* a(i-2)   */
        seq_t prev_gcd = gcd(prevprev, prev);       /* GCD(a(i-2),a(i-1)) */

        seq_t cand = prev + (seq_t)1;
        /* safety guard – should never trigger for valid inputs */
        for (;;) {
            seq_t g = gcd(prev, cand);
            if (g > (seq_t)1 && g != prev_gcd) {
                /* store the new term */
                if (cache_len < MAX_CACHE)
                    cache[cache_len++] = cand;
                else
                    /* if cache overflows, abort – the problem limits n <= MAX_CACHE */
                    abort();
                break;
            }
            cand++;
        }
    }
}

seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0; /* undefined, but defined for completeness */
    ensure_terms(n);
    return cache[n - 1];
}
