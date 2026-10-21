/*
 * A236653  Positive integers n such that n^3 divided by the digital root of n
 *          is a cube.
 * https://oeis.org/A236653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) with a(1)=1, a(2)=8, a(3)=10,
 *            a(4)=19 (OEIS A236653)
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache of terms: seq_cache[i] = a(i) for i>=1 */
static size_t cache_cap = 0;      /* allocated size (at least cache_cap+1 valid elements) */
static size_t cache_size = 0;     /* highest index whose term is currently stored */

seq_t a(idx_t n)
{
    /* undefined for n == 0: return 0 */
    if (n == 0) return 0;

    /* Ensure the buffer is large enough */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) exit(1);          /* abort on allocation failure */
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first four terms if not already done */
    if (cache_size < 4) {
        if (cache_size < 1) seq_cache[1] = (seq_t)1;
        if (cache_size < 2) seq_cache[2] = (seq_t)8;
        if (cache_size < 3) seq_cache[3] = (seq_t)10;
        if (cache_size < 4) seq_cache[4] = (seq_t)19;
        cache_size = 4;
    }

    /* Compute any missing terms up to n using the recurrence */
    for (idx_t i = (idx_t)(cache_size + 1); i <= n; ++i) {
        size_t idx = (size_t)i;
        seq_cache[idx] =
            seq_cache[idx - 1] + seq_cache[idx - 3] - seq_cache[idx - 4];
    }
    cache_size = n;

    return seq_cache[(size_t)n];
}
