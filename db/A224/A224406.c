/*
 * A224406  Number of n X 5 0..1 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 4*a(n-3) + 2*a(n-4) + 8*a(n-5);
 *            Initial terms: a(1)=16, a(2)=101, a(3)=465, a(4)=1928, a(5)=7812
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (cache_size == 0) {
        cache_size = 6;                     /* indices 0..5 */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = 0;          /* unused */
        cache[1] = 16;
        cache[2] = 101;
        cache[3] = 465;
        cache[4] = 1928;
        cache[5] = 7812;
    }
    if (n < cache_size)
        return cache[n];

    /* grow cache up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;
    cache = tmp;
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t t1 = cache[i-1];
        seq_t t2 = cache[i-2];
        seq_t t3 = cache[i-3];
        seq_t t4 = cache[i-4];
        seq_t t5 = cache[i-5];
        seq_t val = 6 * t1 + 4 * t3 + 2 * t4 + 8 * t5;
        val -= 9 * t2;
        cache[i] = val;
    }
    cache_size = new_size;
    return cache[n];
}
