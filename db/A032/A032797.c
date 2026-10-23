/*
 * A032797  Numbers n such that n(n+1)(n+2)...(n+10)
 *          /(n+(n+1)+(n+2)+...+(n+10)) is a multiple of n.
 * https://oeis.org/A032797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) for n > 10 with initial values
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=7, a(7)=8, a(8)=9,
 *            a(9)=10, a(10)=12
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for NULL */

static const seq_t init_seq[11] = { 0, 1, 2, 3, 4, 5,   7, 8, 9, 10, 12 };

/* Cache large enough for the usual test range (≈10 000).  The value
   can be increased if larger ranges are required. */
#define CACHE_SIZE 20001
static seq_t cache[CACHE_SIZE];
static idx_t cache_max = 0;   /* highest index that has been cached (0 = uninitialised) */

static void init_cache(void)
{
    for (idx_t i = 1; i <= 10; ++i)
        cache[i] = init_seq[i];
    cache_max = 10;
}

seq_t a(idx_t n)
{
    if (n <= 10)
        return init_seq[n];

    if (cache_max == 0)
        init_cache();

    if (n <= cache_max)
        return cache[n];

    /* If the requested index is within the cache array, extend it. */
    if (n < CACHE_SIZE) {
        for (idx_t i = cache_max + 1; i <= n; ++i)
            cache[i] = cache[i-1] + cache[i-9] - cache[i-10];
        cache_max = n;
        return cache[n];
    }

    /* For indices beyond CACHE_SIZE, compute on‑the‑fly using a ring buffer
       of size 11 that holds the last 11 terms needed by the recurrence. */
    seq_t ring[11] = { 0 };
    for (idx_t i = 1; i <= 10; ++i)
        ring[i] = init_seq[i];

    for (idx_t i = 11; i <= n; ++i) {
        seq_t val = ring[(i-1) % 11] + ring[(i-9) % 11] - ring[(i-10) % 11];
        ring[i % 11] = val;
    }
    return ring[n % 11];
}
