/*
 * A042550  Numerators of continued fraction convergents to sqrt(804).
 * https://oeis.org/A042550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(804).
   Recurrence: let p_{-1}=1, p_0=28 (the integer part of sqrt(804)).
   For n>=1, let a_n be the nth continued‑fraction coefficient of sqrt(804)
   (a_1=2, a_2=1, a_3=4, a_4=2, a_5=18, a_6=2, a_7=4, a_8=1, a_9=2, a_10=56,
   then the sequence repeats).
   Then p_n = a_n * p_{n-1} + p_{n-2}.  The sequence a(n) returned by this
   function is p_n. */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Periodic part of the continued fraction of sqrt(804). */
static const seq_t period[10] = {2, 1, 4, 2, 18, 2, 4, 1, 2, 56};
static const idx_t period_len = 10;
static const seq_t a0 = 28;

/* Ensure cache contains all terms up to n. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    /* Extend the cached array. */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp)
        abort();            /* allocation failure – cannot proceed */
    cache = tmp;

    idx_t i = cache_len;
    seq_t prev2, prev1;

    /* Determine previous two values for recurrence. */
    if (cache_len == 0) {          /* no previous terms yet */
        prev2 = 0;                /* corresponds to p_{-1} */
        prev1 = 1;                /* corresponds to p_{0-1} for next iteration */
    } else if (cache_len == 1) {   /* only p_0 is known */
        prev2 = 1;                /* p_{-1} */
        prev1 = cache[0];         /* p_0 */
    } else {                       /* at least two terms known */
        prev2 = cache[cache_len - 2];
        prev1 = cache[cache_len - 1];
    }

    /* Compute new terms. */
    for (; i <= n; ++i) {
        seq_t a = (i == 0) ? a0 : period[(i - 1) % period_len];
        seq_t curr = a * prev1 + prev2;
        cache[i] = curr;
        prev2 = prev1;
        prev1 = curr;
    }
    cache_len = n + 1;
}

/* Main function. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
