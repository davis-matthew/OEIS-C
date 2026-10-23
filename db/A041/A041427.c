/*
 * A041427  Denominators of continued fraction convergents to sqrt(229).
 * https://oeis.org/A041427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 3420*a(n-5) + a(n-10) for n > 9 Initial values (0 ≤ n ≤
 *            9): a(0)=1, a(1)=7, a(2)=8, a(3)=15, a(4)=113, a(5)=3405,
 *            a(6)=23948, a(7)=27353, a(8)=51301, a(9)=386460
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_CACHE_SIZE 20001   /* large enough for n up to ~1e4 */

static seq_t cache[MAX_CACHE_SIZE];
static idx_t cache_max = 0;
static bool cache_ready = false;

/* Compute the n-th term of A041427 */
seq_t a(idx_t n)
{
    /* Initialise the cache once */
    if (!cache_ready) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)113;
        cache[5] = (seq_t)3405;
        cache[6] = (seq_t)23948;
        cache[7] = (seq_t)27353;
        cache[8] = (seq_t)51301;
        cache[9] = (seq_t)386460;
        cache_max = 9;
        cache_ready = true;
    }

    if (n <= cache_max)          /* already computed */
        return cache[n];

    /* Make sure we have enough space (caller is responsible for n < MAX_CACHE_SIZE) */
    /* Compute all terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)3420 * cache[i - 5] + cache[i - 10];
    }
    cache_max = n;
    return cache[n];
}
