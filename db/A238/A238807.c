/*
 * A238807  Number of n X 3 0..2 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of the sum of
 *          elements above it, modulo 3.
 * https://oeis.org/A238807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) for n>9 Initial values: a(1)=4, a(2)=15,
 *            a(3)=48, a(4)=118, a(5)=254, a(6)=498, a(7)=916, a(8)=1605,
 *            a(9)=2702
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 109, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_CACHE 20000   /* sufficient for the requested n (≈10 000) */

static seq_t cache[MAX_CACHE + 1];
static idx_t cache_size = 0;

static void init_cache(void)
{
    if (cache_size != 0) return;
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)15;
    cache[3] = (seq_t)48;
    cache[4] = (seq_t)118;
    cache[5] = (seq_t)254;
    cache[6] = (seq_t)498;
    cache[7] = (seq_t)916;
    cache[8] = (seq_t)1605;
    cache[9] = (seq_t)2702;
    cache_size = 9;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; a(0) unused */
    init_cache();

    if (n <= cache_size) {
        return cache[(size_t)n];
    }

    /* extend cache until index n (should not exceed MAX_CACHE) */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        seq_t t5 = cache[i - 5];
        seq_t t6 = cache[i - 6];
        seq_t t7 = cache[i - 7];

        seq_t val = 7 * t1;
        val -= 21 * t2;
        val += 35 * t3;
        val -= 35 * t4;
        val += 21 * t5;
        val -= 7 * t6;
        val += t7;
        cache[i] = val;
    }
    cache_size = (size_t)n;
    return cache[(size_t)n];
}
