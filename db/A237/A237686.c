/*
 * A237686  The number of P-positions in the game of Nim with up to four
 *          piles, allowing for piles of zero, such that the total number of
 *          objects in all piles doesn't exceed 2n.
 * https://oeis.org/A237686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 7, a(2n+1) = 7*a(n) + a(n-1), a(2n+2) = a(n+1)
 *            + 7*a(n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static seq_t cache[10001];          /* computed values, up to n = 10000 */
static unsigned char done[10001];   /* 1 if cache[i] is initialized */
static size_t cache_len = 0;        /* highest index initialized (0‑based) */

/* Compute a(n) using the recurrence, populating the cache as needed. */
seq_t a(idx_t n)
{
    if (n > 10000) {  /* guard against out‑of‑bounds request */
        /* For n beyond 10000 the function may not be needed. */
        /* Calls with n > 10000 are not guaranteed to be correct. */
        return (seq_t)0;
    }

    /* Build cache up to index n */
    for (size_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)7;
        } else if ((i & 1u) == 0) {            /* i = 2k  */
            idx_t k = i >> 1;
            cache[i] = cache[k] + (seq_t)7 * cache[k - 1];
        } else {                               /* i = 2k+1 */
            idx_t k = (i - 1) >> 1;
            cache[i] = (seq_t)7 * cache[k] + cache[k - 1];
        }
        done[i] = 1;
    }
    cache_len = n + 1;   /* next compute starts after n */
    return cache[n];
}
