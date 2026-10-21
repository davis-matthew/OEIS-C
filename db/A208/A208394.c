/*
 * A208394  Number of 3 X n 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A208394
 *
 * Empirical recurrence (valid for n ≥ 7):
 *   a(n) = 22*a(n-1) + 80*a(n-2) + 40*a(n-3) - 48*a(n-4)
 *
 * Initial values (offset 1):
 *   a(1)=5,  a(2)=122,  a(3)=3042,  a(4)=76806,
 *   a(5)=1937736,  a(6)=48890520
 *
 * The function keeps a memoisation cache so that calls can be made
 * in any order and are O(1) after the first computation of each term.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* zero‑based cache: cache[0] = a(1) */
static idx_t cache_len = 0;    /* number of terms currently cached */

/* initialise or extend the cache up to term 'up_to' (1‑based) */
static void extend_cache(idx_t up_to)
{
    if (cache_len >= up_to)
        return;               /* already computed */

    /* Allocate enough space for new terms */
    size_t new_len = (size_t)up_to;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        abort();            /* allocation failure */
    }
    cache = tmp;

    /* Initial values for the first six terms */
    const seq_t init[6] = {
        (seq_t)5,          /* a(1) */
        (seq_t)122,        /* a(2) */
        (seq_t)3042,       /* a(3) */
        (seq_t)76806,      /* a(4) */
        (seq_t)1937736,    /* a(5) */
        (seq_t)48890520    /* a(6) */
    };

    /* Fill missing entries */
    for (idx_t i = cache_len; i < up_to; ++i) {   /* i is zero‑based: i = n-1 */
        if (i < 6) {
            cache[i] = init[i];
        } else {
            /* recurrence for n = i+1, i >= 6 (i.e., n >= 7) */
            cache[i] =
                (seq_t)22 * cache[i-1] +
                (seq_t)80 * cache[i-2] +
                (seq_t)40 * cache[i-3] -
                (seq_t)48 * cache[i-4];
        }
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for offset 1, safe default */
        return (seq_t)0;
    extend_cache(n);
    return cache[n-1];      /* cache is zero‑based */
}
