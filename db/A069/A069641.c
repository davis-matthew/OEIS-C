/*
 * A069641  Let M_n be the n X n matrix with M_n(i,j)=1/(i+j+2); then
 *          a(n)=1/det(M_n).
 * https://oeis.org/A069641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4 * Π_{k=2}^{n} [(2k+2) * C(2k+1, k-1)^2] (recursive)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) return 0; // offset starts at 1; behaviour for n<1 irrelevant

    if (n > cache_size) {
        /* Resize cache */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) abort(); /* allocation failed */
        cache = new_cache;

        /* Compute new values up to n */
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            if (k == 1) {
                cache[1] = (seq_t)4;
            } else {
                seq_t c = binom(2 * k + 1, k - 1);          /* C(2k+1, k-1) */
                seq_t tmp = c * c;                          /* square */
                tmp *= (seq_t)(2 * k + 2);                  /* multiply by 2k+2 */
                cache[k] = cache[k - 1] * tmp;              /* recurrence */
            }
        }
        cache_size = n;
    }
    return cache[n];
}
