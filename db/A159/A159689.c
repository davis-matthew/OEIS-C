/*
 * A159689  Fixed point of the morphism 0 -> 0,1,0; 1 -> 1,1; starting from
 *          a(0)=0.
 * https://oeis.org/A159689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th term of the fixed point of the morphism * 0 →
 *            0,1,0 and 1 → 1,1, starting from a(0)=0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* each call expands a cached array until it covers the requested index.
 * The array holds the infinite word prefix in 0/1 form. */
static seq_t *cached = NULL;
static idx_t cached_len = 0;

/* ensure that cached has length > n, expanding by the morphism each time */
static void ensure_cached(idx_t n)
{
    if (cached_len > n)
        return;
    if (cached_len == 0) {          /* first time */
        cached_len = 1;
        cached = (seq_t *)malloc(sizeof(seq_t));
        cached[0] = 0;
    }
    while (cached_len <= n) {
        /* compute new length */
        idx_t new_len = 0;
        for (idx_t i = 0; i < cached_len; ++i)
            new_len += (cached[i] == 0) ? (idx_t)3 : (idx_t)2;

        /* build new array */
        seq_t *next = (seq_t *)malloc(sizeof(seq_t) * new_len);
        idx_t pos = 0;
        for (idx_t i = 0; i < cached_len; ++i) {
            if (cached[i] == 0) {            /* 0 → 0,1,0 */
                next[pos++] = 0;
                next[pos++] = 1;
                next[pos++] = 0;
            } else {                         /* 1 → 1,1 */
                next[pos++] = 1;
                next[pos++] = 1;
            }
        }
        free(cached);
        cached = next;
        cached_len = new_len;
    }
}

seq_t a(idx_t n)
{
    ensure_cached(n);
    return cached[n];
}
