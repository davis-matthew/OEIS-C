/*
 * A169267  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -78*a(n-29) + 12*Sum_{k=1..28} a(n-k) (n ≥ 29) a(0) = 1,
 *            a(1) = 14, ... , a(28) = 14*13^(i-1) for i = 1..28 (the first 28
 *            non‑zero terms follow 14·13^(i-1) ) This implementation uses
 *            memoisation and a rolling sum so that a(n) can be requested in
 *            any order and computed in O(n) time overall
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* array of already computed values   */
    static size_t cap        = 0;      /* allocated capacity                 */
    static size_t max_computed = 0;    /* number of values that are valid    */
    static int base_ready     = 0;      /* base values 0..28 are ready?       */
    static seq_t last28_sum   = 0;      /* sum of the last 28 terms (for recurrence) */

    /* --- Ensure enough storage for the requested term and for the
           mandatory first 29 terms (0..28) --- */
    if (n + 1 > cap) {
        size_t new_cap = (cap == 0) ? (n + 1) : cap;
        if (new_cap < n + 1) new_cap = n + 1;
        if (new_cap < 29)    new_cap = 29;   /* we need at least 29 entries for base */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);      /* quit on allocation failure */
        if (new_cap > cap)
            memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* --- Initialise the base values once only --- */
    if (!base_ready) {
        cache[0] = (seq_t)1;
        seq_t v  = (seq_t)14;     /* a(1) */
        cache[1] = v;
        for (idx_t i = 2; i <= 28; ++i) {
            v = v * (seq_t)13;    /* 13 * previous term */
            cache[i] = v;
        }
        /* compute sum of a(1)..a(28) for the recurrence */
        last28_sum = (seq_t)0;
        for (idx_t i = 1; i <= 28; ++i)
            last28_sum += cache[i];
        base_ready = 1;
        max_computed = 29;         /* indices 0..28 are now ready */
    }

    /* --- Return early for indices inside the base range --- */
    if (n < 29)
        return cache[n];

    /* --- Compute any new terms up to n (if required) --- */
    if (max_computed <= n) {
        for (idx_t i = max_computed; i <= n; ++i) {
            /* a(i) = -78*a(i-29) + 12*sum_{k=1}^{28} a(i-k) */
            seq_t term   = (seq_t)12 * last28_sum;
            term         -= (seq_t)78 * cache[i - 29];
            cache[i]      = term;

            /* Update rolling sum of the last 28 terms */
            last28_sum += cache[i];
            last28_sum -= cache[i - 28];
        }
        max_computed = n + 1;
    }
    return cache[n];
}
