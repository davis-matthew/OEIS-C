/*
 * A116703  Number of permutations of length n which avoid the patterns 231,
 *          4123.
 * https://oeis.org/A116703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(1)=1, a(2)=2, a(3)=5; for n≥4 *
 *            a(n)=3*a(n-3)−5*a(n-2)+4*a(n-1) (recurrence from the G.f.) *
 *            This implements the OEIS entry A116703
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cached values for a(1)…a(k); cache[0] unused */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* one past the highest computed index */

    /* initialise base cases (a(1),a(2),a(3)) */
    if (cache_len <= 3) {
        idx_t needed = 4;                 /* indices 0 … 3 */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) { cache = NULL; return 0; } /* allocation failure */
        cache = tmp;
        cache_len = needed;
        cache[1] = 1;   /* a(1) */
        cache[2] = 2;   /* a(2) */
        cache[3] = 5;   /* a(3) */
    }

    /* already computed? */
    if (n < cache_len) return cache[n];

    /* grow cache to hold up to a(n) */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) { cache = NULL; return 0; }   /* allocation failure */
    cache = tmp;

    /* compute new terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t t1 = cache[i-3] * (seq_t)3;
        seq_t t2 = cache[i-1] * (seq_t)4;
        seq_t t3 = cache[i-2] * (seq_t)5;
        cache[i] = t1 + t2 - t3;   /* guaranteed non‑negative for this seq */
    }

    cache_len = n + 1;
    return cache[n];
}
