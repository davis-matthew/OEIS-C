/*
 * A240478  Number of n X 2 0..1 arrays with no element equal to more than two
 *          horizontal or vertical neighbors, with new values 0..1 introduced
 *          in row major order.
 * https://oeis.org/A240478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 2*a(n-4), a(1)=2, a(2)=8, a(3)=25,
 *            a(4)=81
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;  /* number of filled entries (n==cache_len) */
static idx_t  cache_cap = 0;  /* current allocation size */

static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n<1, but not used */
    ensure_capacity(n);

    if (cache_len < n) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)
                val = (seq_t)2;
            else if (i == 2)
                val = (seq_t)8;
            else if (i == 3)
                val = (seq_t)25;
            else if (i == 4)
                val = (seq_t)81;
            else {
                seq_t a1 = cache[i - 2];   /* a(i-1) */
                seq_t a2 = cache[i - 3];   /* a(i-2) */
                seq_t a4 = cache[i - 5];   /* a(i-4) */
                val = (seq_t)3 * a1 + a2 - (seq_t)2 * a4;
            }
            cache[i - 1] = val;
        }
        cache_len = n;
    }
    return cache[n - 1];
}
