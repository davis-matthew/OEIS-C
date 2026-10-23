/*
 * A048922  Indices of 9-gonal numbers which are also octagonal.
 * https://oeis.org/A048922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 674*a(n-1) - a(n-2) - 240, with a(1)=1, a(2)=425. The
 *            first seven terms are given explicitly to avoid overflow in
 *            64‑bit builds
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* undefined for offset 1 */

    static seq_t *cache = NULL;              /* cache[i] = a(i+1) */
    static size_t cache_size = 0;            /* number of stored terms   */

    if (cache_size < (size_t)n) {
        size_t new_size = (size_t)n;

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();                         /* allocation failure */

        cache = tmp;

        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)425;
            } else if (i == 2) {
                cache[i] = (seq_t)286209;
            } else if (i == 3) {
                cache[i] = (seq_t)192904201;
            } else if (i == 4) {
                cache[i] = (seq_t)130017145025;
            } else if (i == 5) {
                cache[i] = (seq_t)87631362842409ULL;
            } else if (i == 6) {
                cache[i] = (seq_t)59063408538638401ULL;
            } else {
                cache[i] = (seq_t)674 * cache[i-1]          /* requires only + - * / % */
                         - cache[i-2]
                         - (seq_t)240;
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n - 1];
}
