/*
 * A207174  Number of 6 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A207174:
 * a(n) = a(n-1) + 21*a(n-3) + 5*a(n-4) + 5*a(n-5)
 *        - 75*a(n-6) - 50*a(n-7) + 125*a(n-9)   for n>11
 * initial values:
 * a(1)=12, a(2)=144, a(3)=396, a(4)=1989, a(5)=6513,
 * a(6)=15789, a(7)=54208, a(8)=182081, a(9)=515760,
 * a(10)=1608288, a(11)=5222049
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* out of range – not used in tests */

    /* Initial values for n = 1 … 11 (index 0 unused). */
    static const seq_t init[12] = {
        0ULL,
        12ULL,   144ULL,   396ULL,   1989ULL,
        6513ULL, 15789ULL, 54208ULL, 182081ULL,
        515760ULL, 1608288ULL, 5222049ULL
    };

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_up_to = 0;    /* largest n already computed */

    /* Ensure capacity for index n */
    if (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        if (i <= 11) {
            cache[i] = init[i];
        } else {
            seq_t sum = cache[i-1];
            sum += 21 * cache[i-3];
            sum += 5  * cache[i-4];
            sum += 5  * cache[i-5];
            sum -= 75 * cache[i-6];
            sum -= 50 * cache[i-7];
            sum += 125 * cache[i-9];
            cache[i] = sum;
        }
    }
    cached_up_to = n;

    return cache[n];
}
