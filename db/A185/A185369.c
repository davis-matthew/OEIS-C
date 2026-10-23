/*
 * A185369  Number of simple labeled graphs on n nodes of degree 1 or 2
 *          without cycles.
 * https://oeis.org/A185369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A185369: Number of simple labeled graphs on n nodes of degree 1 or 2 without cycles.
 * Recurrence (D‑finite):  
 *    2*a(n) + 4*(1‑n)*a(n‑1) + 2*(n‑1)*(n‑3)*a(n‑2) + (n‑1)*(n‑2)*a(n‑3) = 0
 *    =>  a(n) = 2*(n‑1)*a(n‑1) – (n‑1)*(n‑3)*a(n‑2) – ((n‑1)*(n‑2)*a(n‑3))/2
 * Initial values: a(0)=1, a(1)=0, a(2)=1.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    /* Allocate and fill cache up to n */
    if (n >= cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) /* allocation failed – return 0 as an error code */
            return 0;
        cache = new_cache;

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 1;
            } else if (i == 1) {
                cache[i] = 0;
            } else if (i == 2) {
                cache[i] = 1;
            } else {
                /* term1 = 2*(i-1)*a(i-1) */
                seq_t term1 = (seq_t)2 * ((seq_t)(i - 1) * cache[i - 1]);

                /* term2 = (i-1)*(i-3)*a(i-2) */
                seq_t term2 = ((seq_t)(i - 1) * (seq_t)(i - 3)) * cache[i - 2];

                /* term3 = ((i-1)*(i-2)*a(i-3))/2 */
                seq_t term3 = (((seq_t)(i - 1) * (seq_t)(i - 2)) * cache[i - 3]) >> 1;

                cache[i] = term1 - term2 - term3;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
