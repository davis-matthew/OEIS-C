/*
 * A164605  a(n) = ((1+4*sqrt(2))*(4+2*sqrt(2))^n +
 *          (1-4*sqrt(2))*(4-2*sqrt(2))^n)/2.
 * https://oeis.org/A164605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) for n > 1, a(0) = 1, a(1) = 20
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;       /* cache[0..len-1] holds a(i) */
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;

    /* initialise cache if empty */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) exit(1);          /* abort on allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)20;
        cache_len = 2;
    }

    /* extend cache up to required n */
    if (n >= cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = ((seq_t)8 * cache[i - 1] - (seq_t)8 * cache[i - 2]);
        }
        cache_len = n + 1;
    }

    return cache[n];
}
