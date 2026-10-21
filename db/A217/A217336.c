/*
 * A217336  a(n) = 3^(-1+floor(n/2))*A(n), where A(n) = 3*A(n-1) + A(n-2) -
 *          A(n-3)/3 with A(0)=A(1)=3, A(2)=11.
 * https://oeis.org/A217336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(floor(n/2)-1) * A(n) (A(n) satisfies
 *            A(n)=3*A(n-1)+A(n-2)-A(n-3)/3 with A(0)=A(1)=3, A(2)=11). The
 *            integer recurrence for a(n) is a(0)=1, a(1)=1, a(2)=11
 *            a(n)=9*a(n-1)+3*a(n-2)-3*a(n-3) if n is even
 *            a(n)=3*a(n-1)+3*a(n-2)-a(n-3) if n is odd
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of entries already computed */

static void ensure_up_to(idx_t n)
{
    if (n < cache_size) return;         /* already computed */

    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();            /* out of memory */

    /* compute values for indices cache_size .. n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i == 0 || i == 1) {
            val = 1;                    /* a(0)=a(1)=1 */
        } else if (i == 2) {
            val = 11;                   /* a(2)=11 */
        } else {
            if ((i & 1) == 0) {         /* even n */
                val = 9 * new_cache[i-1] + 3 * new_cache[i-2] - 3 * new_cache[i-3];
            } else {                    /* odd n */
                val = 3 * new_cache[i-1] + 3 * new_cache[i-2] - new_cache[i-3];
            }
        }
        new_cache[i] = val;
    }

    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[n];
}
