/*
 * A129346  a(2n) = A100525(n), a(2n+1) = A001653(n+1); a Pellian-related
 *          sequence.
 * https://oeis.org/A129346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 4, a(1) = 5, a(2) = 22, a(3) = 29 a(n) = 6*a(n-2) -
 *            a(n-4) for n > 3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* number of slots allocated (>= current index + 1) */

    /* initialise cache with the first four terms */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (cache == NULL) return (seq_t)0;   /* allocation failed – return 0 */
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)22;
        cache[3] = (seq_t)29;
    }

    /* if request exceeds cached size, grow and compute missing terms */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return (seq_t)0;      /* allocation failed – return 0 */
        cache = tmp;
        cap = new_cap;

        /* compute terms from the first missing index up to n */
        for (idx_t i = 4; i <= n; ++i) {
            cache[i] = (seq_t)6 * cache[i-2] - cache[i-4];
        }
    }

    return cache[n];
}
