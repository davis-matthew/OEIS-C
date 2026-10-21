/*
 * A098144  a(n) = smallest number greater than a(n-1) having a largest proper
 *          divisor that is greater than and coprime to a(n-1); a(1) = 1.
 * https://oeis.org/A098144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 4, a(2*n) = a(2*n-1) + 1 (n > 1), a(2*n+1) =
 *            3*(a(2*n)/2 + 2 - 0^(a(2*n) mod 4)) (n > 1)
 * Range:     u8 n <= 16, u16 n <= 42, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence is used; a(2) is the only initial value needed in addition to a(1). */

#include <stdlib.h>
#include <assert.h>

static seq_t *a_cache = NULL;   /* 1‑based array: a_cache[i] = a(i)   */
static size_t cache_cap = 0;    /* allocated size of a_cache          */
static idx_t  cache_len = 0;    /* indices 1..cache_len are computed */

/* Ensure that a_cache has room for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;
    cache_cap = new_cap;
}

/* Main function computing a(n).  Caller may request any n ≥ 1, in any order. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for n < 1 */

    ensure_capacity(n);

    /* initialise the cache on first call */
    if (cache_len == 0) {
        a_cache[1] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len < 2) {
        a_cache[2] = (seq_t)4;
        cache_len = 2;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if ((i & 1U) == 0) { /* i is even: a(i) = a(i-1) + 1 */
            a_cache[i] = a_cache[i - 1] + (seq_t)1;
        } else {            /* i is odd (>1): use recurrence */
            seq_t prev = a_cache[i - 1];           /* a(2n) */
            seq_t half = prev / (seq_t)2;          /* prev is even   */
            seq_t sub  = (prev % (seq_t)4 == (seq_t)0) ? (seq_t)1 : (seq_t)0;
            a_cache[i] = (seq_t)3 * (half + (seq_t)2 - sub);
        }
    }

    cache_len = n;
    return a_cache[n];
}
