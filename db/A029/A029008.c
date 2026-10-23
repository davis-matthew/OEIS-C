/*
 * A029008  Expansion of 1/((1-x)*(1-x^2)*(1-x^4)*(1-x^7)).
 * https://oeis.org/A029008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-1)+a(n-2)-a(n-3)+a(n-4)-a(n-5)-a(n-6)+2*a(n-7)-a(n-8)-a(n-9)+a(n-10)-a(n-11)+a(n-12)+a(n-13)-a(n-14)
 *            Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=2, a(4)=4, a(5)=4,
 *            a(6)=6, a(7)=7, a(8)=10, a(9)=11, a(10)=14, a(11)=16, a(12)=20,
 *            a(13)=22 This implementation uses the recurrence to compu...
 * Range:     u8 n <= 37, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static signed long long *cache = NULL;   /* cached sequence in signed 64‑bit */
static idx_t cache_len = 0;             /* current capacity of cache */

/* pre‑computed initial terms */
static const signed long long init[14] = {
    1, 1, 2, 2, 4, 4, 6, 7,
    10, 11, 14, 16, 20, 22
};

seq_t a(idx_t n)
{
    /* grow cache to accommodate n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        signed long long *new_cache = (signed long long *)realloc(cache,
                                    new_len * sizeof(signed long long));
        if (!new_cache) {
            /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = new_cache;

        /* fill newly added entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 14) {
                cache[i] = init[i];
            } else {
                /* recurrence with signed arithmetic to avoid underflow */
                signed long long sum = 0;
                sum +=  cache[i-1];
                sum +=  cache[i-2];
                sum -=  cache[i-3];
                sum +=  cache[i-4];
                sum -=  cache[i-5];
                sum -=  cache[i-6];
                sum +=  2 * cache[i-7];
                sum -=  cache[i-8];
                sum -=  cache[i-9];
                sum +=  cache[i-10];
                sum -=  cache[i-11];
                sum +=  cache[i-12];
                sum +=  cache[i-13];
                sum -=  cache[i-14];
                cache[i] = sum;
            }
        }
        cache_len = new_len;
    }
    return (seq_t)cache[n];
}
