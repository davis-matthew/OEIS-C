/*
 * A090992  Number of meaningful differential operations of the n-th order on
 *          the space R^7.
 * https://oeis.org/A090992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = a(n+3) + 3*a(n+2) - 2*a(n+1) - a(n) a(1) = 7, a(2) =
 *            13, a(3) = 24, a(4) = 45
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 33, u64 n <= 68, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_upto = 0;

static void ensure_cache(idx_t n)
{
    if (n == 0) return;

    if (cache_len == 0) {                  /* first time : allocate */
        size_t new_len = (size_t)(n + 5);
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!cache) exit(1);              /* allocation failure */
        cache_len = new_len;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)24;
        cache[4] = (seq_t)45;
        computed_upto = 4;
    }
    else if ((size_t)n + 1 > cache_len) {   /* grow if needed */
        size_t new_len = (size_t)(n + 5);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_len = new_len;
    }

    if (n <= computed_upto) return;

    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        seq_t term = cache[i-1];
        term += ((seq_t)3) * cache[i-2];
        term -= ((seq_t)2) * cache[i-3];
        term -= cache[i-4];
        cache[i] = term;
    }
    computed_upto = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_cache(n);
    return cache[n];
}
