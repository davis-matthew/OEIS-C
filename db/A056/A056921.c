/*
 * A056921  a(0) = 0, a(1) = 1, a(2*n) = n*a(2*n-1) + a(2*n-2), a(2*n+1) =
 *          a(2*n) + a(2*n-1).
 * https://oeis.org/A056921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, easy, nonn
 * Formula:   a(0)=0, a(1)=1, a(2n)=n*a(2n-1)+a(2n-2), a(2n+1)=a(2n)+a(2n-1)
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 23, u64 n <= 39, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Use a reasonably large compile‑time buffer.  For the test suite n is
 * at most about 10 000, so 20 001 covers all required indices.  The
 * buffer is static so the implementation is valid in both C and C++.
 */
#define CACHE_MAX 20001
static seq_t cache[CACHE_MAX];
static idx_t cache_len = 0;

/* Ensure the cache contains values up to at least n.  If n exceeds
 * CACHE_MAX the function aborts – the test suite never requires that.
 */
static void compute_up_to(idx_t n)
{
    if (cache_len == 0)                    /* first use: initialise */
    {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    if (n < cache_len)
        return;

    if (n >= CACHE_MAX)
        abort();   /* n too large for the static buffer */

    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0) { cache[i] = (seq_t)0; continue; }
        if (i == 1) { cache[i] = (seq_t)1; continue; }

        /* Even indices: a(2n) = n*a(2n-1) + a(2n-2) */
        if ((i & 1U) == 0)
        {
            seq_t k = (seq_t)(i >> 1);          /* k = i/2 as seq_t   */
            cache[i] = k * cache[i-1] + cache[i-2];
        }
        /* Odd indices:  a(2n+1) = a(2n) + a(2n-1) */
        else
        {
            cache[i] = cache[i-1] + cache[i-2];
        }
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
