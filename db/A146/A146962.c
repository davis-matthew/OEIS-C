/*
 * A146962  a(n) = 10*a(n-1) - 19*a(n-2) with a(0)=1, a(1)=5.
 * https://oeis.org/A146962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 19*a(n-2), a(0)=1, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
    }

    /* Grow cache if needed */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure: fall back to on‑the‑fly computation */
            seq_t a0 = (seq_t)1;
            seq_t a1 = (seq_t)5;
            seq_t an;
            for (idx_t i = 2; i <= n; ++i) {
                an = ((seq_t)10) * a1 - ((seq_t)19) * a0;
                a0 = a1;
                a1 = an;
            }
            return an;
        }
        cache = tmp;
        cache_len = new_len;
        for (idx_t i = old_len; i < new_len; ++i) {
            cache[i] = ((seq_t)10) * cache[i - 1] - ((seq_t)19) * cache[i - 2];
        }
    }

    return cache[n];
}
