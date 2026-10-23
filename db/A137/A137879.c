/*
 * A137879  Numbers k such that k^2 is a 17-gonal number.
 * https://oeis.org/A137879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 482*a(n-2) - a(n-4) for n >= 5 initial values: a(1)=1,
 *            a(2)=133, a(3)=615, a(4)=64107
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache[0] unused, indices start at 1 */
static idx_t cache_len = 0;          /* allocated size (element count) */

static void ensure_cache(idx_t n)
{
    if (cache_len <= n) {
        /* Allocate array holding indices 0..n (size n+1) */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);  /* allocation failed */

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 1) {
                tmp[1] = (seq_t)1;
            } else if (i == 2) {
                tmp[2] = (seq_t)133;
            } else if (i == 3) {
                tmp[3] = (seq_t)615;
            } else if (i == 4) {
                tmp[4] = (seq_t)64107;
            } else { /* i >= 5 */
                tmp[i] = (seq_t)482 * tmp[i - 2] - tmp[i - 4];
            }
        }
        cache = tmp;
        cache_len = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined but safe */
    ensure_cache(n);
    return cache[n];
}
