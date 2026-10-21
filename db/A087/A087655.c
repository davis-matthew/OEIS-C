/*
 * A087655  a(n) = a(abs(n - a(n-1))) + a(abs(n - a(n-2))) + a(abs(n -
 *          a(n-3))) with a(1)=a(2)=a(3)=1.
 * https://oeis.org/A087655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(|n - a(n-1)|) + a(|n - a(n-2)|) + a(|n - a(n-3)|) with
 *            a(0) = 1, a(1) = a(2) = a(3) = 1. The value for a(0) is defined
 *            so that the absolute differences appearing in the recurrence are
 *            well‑defined
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache   = NULL;   /* memoisation table for a(n)   */
static size_t  cap    = 0;      /* current capacity of cache    */

/* Ensure that cache has space for indices up to n */
static void ensure_cap(idx_t n)
{
    if ((size_t)n + 1 <= cap) return;

    size_t newcap = cap ? cap * 2 : 16;
    while (newcap <= (size_t)n + 1) newcap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed – terminate   */

    /* initialise the new part to 0 (sentinel) */
    memset(tmp + cap, 0, (newcap - cap) * sizeof(seq_t));

    cache = tmp;
    cap   = newcap;
}

/* Recursive computation with memoisation */
seq_t a(idx_t n)
{
    if (n == 0) return 1;                /* base case: a(0) = 1 */

    ensure_cap(n);

    /* Return memoised value if present */
    if (cache[n]) return cache[n];

    /* Initial conditions for 1 ≤ n ≤ 3 */
    if (n <= 3) {
        cache[n] = 1;
        return 1;
    }

    /* Values needed for the recurrence */
    seq_t v1 = a(n - 1);
    seq_t v2 = a(n - 2);
    seq_t v3 = a(n - 3);
    seq_t n_val = (seq_t)n;

    /* Compute the absolute differences */
    seq_t d1 = v1 > n_val ? v1 - n_val : n_val - v1;
    seq_t d2 = v2 > n_val ? v2 - n_val : n_val - v2;
    seq_t d3 = v3 > n_val ? v3 - n_val : n_val - v3;

    /* Recurse on the obtained indices and sum the results */
    seq_t result = a((idx_t)d1) + a((idx_t)d2) + a((idx_t)d3);

    cache[n] = result;
    return result;
}
