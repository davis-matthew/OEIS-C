/*
 * A138587  The union of all entries of A024495, A131708 and A024493 sorted
 *          into natural order.
 * https://oeis.org/A138587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -a(n-1)+a(n-2)+a(n-3)+2*a(n-4)+2*a(n-5) for n > 6 initial
 *            terms: a(0)=0, a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=6, a(6)=10
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 0;

    /* Ensure we have room for the first seven terms */
    if (cap < 7) {
        size_t newcap = cap ? cap : 64;
        while (newcap < 7) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)6;
        cache[6] = (seq_t)10;
        last = 6;
    }

    /* If n is small, return the pre‑calculated value */
    if (n <= 6) {
        return cache[(size_t)n];
    }

    /* Grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Compute terms up to n in increasing order */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* recurrence: a(i) = -a(i-1) + a(i-2) + a(i-3) + 2*a(i-4) + 2*a(i-5) */
        seq_t t = cache[(size_t)(i - 2)] + cache[(size_t)(i - 3)];
        t += cache[(size_t)(i - 4)] << 1;
        t += cache[(size_t)(i - 5)] << 1;
        t -= cache[(size_t)(i - 1)];
        cache[(size_t)i] = t;
    }
    last = n;
    return cache[(size_t)n];
}
