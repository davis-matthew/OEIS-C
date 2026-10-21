/*
 * A221516  Number of 0..n arrays of length 5 with each element differing from
 *          at least one neighbor by 2 or more, starting with 0.
 * https://oeis.org/A221516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) for
 *            n > 8, with initial values a(1)=0, a(2)=3, a(3)=30, a(4)=130,
 *            a(5)=381, a(6)=884, a(7)=1765, a(8)=3174
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) abort();         /* allocation failed */
        cache = tmp;
        /* initial values a(1) .. a(8) */
        const seq_t base[8] = {0, 3, 30, 130, 381, 884, 1765, 3174};

        for (idx_t i = cache_size; i < n; ++i) {
            if (i < 8) {
                cache[i] = base[i];
            } else {
                cache[i] = (seq_t)5 * cache[i - 1]
                         - (seq_t)10 * cache[i - 2]
                         + (seq_t)10 * cache[i - 3]
                         - (seq_t)5  * cache[i - 4]
                         + cache[i - 5];
            }
        }
        cache_size = n;
    }
    return cache[n - 1];
}
