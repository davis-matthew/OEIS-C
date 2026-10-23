/*
 * A170626  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A170626
    a(0) = 1
    for 1 <= n < 48 :  a(n) = 41 * 40^(n-1)
    for n >= 48    :  a(n) = 39 * (a(n-1)+...+a(n-47)) - 780 * a(n-48)
    (the recurrence follows from the rational generating function 1/(1-39t-...-39t^47+780t^48))
*/

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* memoised values */
static size_t cached = 0;            /* number of computed values */
static bool   init_done = false;     /* initial 48 values ready? */
static seq_t  sum47 = 0;             /* sum of last 47 values, for recurrence */

/* initialise the first 48 terms (0..47) */
static void init_initial_terms(void)
{
    if (init_done)
        return;
    init_done = true;

    /* allocate space for 48 entries */
    cache   = (seq_t *)malloc(48 * sizeof(seq_t));
    cached  = 48;                     /* indices 0..47 are valid */

    cache[0] = (seq_t)1;              /* a(0) = 1 */
    cache[1] = (seq_t)41;             /* a(1) = 41 */
    for (idx_t i = 2; i < 48; ++i)
        cache[i] = cache[i - 1] * (seq_t)40;   /* a(i) = a(i-1)*40 */

    /* compute sum of a(1)..a(47) for the recurrence */
    sum47 = (seq_t)0;
    for (idx_t i = 1; i < 48; ++i)
        sum47 += cache[i];
}

/* compute a(n) for arbitrary n >= 0 */
seq_t a(idx_t n)
{
    if (!init_done)                     /* ensure first 48 terms are ready */
        init_initial_terms();

    if (n < cached)                     /* already computed */
        return cache[n];

    /* extend the cache up to n */
    seq_t *new_cache = (seq_t *)realloc(cache,
                                        (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache)                     /* allocation failed – abort */
        return (seq_t)0;
    cache = new_cache;
    /* fill new indices using the linear recurrence */
    for (idx_t i = cached; i <= n; ++i)
    {
        /* a(i) = 39 * sum47 - 780 * a(i-48)  (for i >= 48) */
        seq_t term = sum47 * (seq_t)39 - cache[i - 48] * (seq_t)780;
        cache[i] = term;

        /* update sum47 for the next step: remove a(i-47), add a(i) */
        if (i >= 47) /* i-47 is always >= 0 for i >= 47 */
            sum47 = sum47 + term - cache[i - 47];
    }
    cached = (size_t)(n + 1);
    return cache[n];
}
