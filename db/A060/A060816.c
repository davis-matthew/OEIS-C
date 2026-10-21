/*
 * A060816  a(0) = 1; a(n) = (5*3^(n-1) - 1)/2 for n > 0.
 * https://oeis.org/A060816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 1 with a(0) = 1, a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static idx_t a_capacity = 0;
static idx_t a_len     = 0;

seq_t a(idx_t n)
{
    if (a_len == 0) {
        a_capacity = 4;
        a_cache = (seq_t *)realloc(NULL, a_capacity * sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)2;
        a_len     = 2;
    }

    if (n < a_len)
        return a_cache[n];

    if (n >= a_capacity) {
        while (a_capacity <= n) a_capacity <<= 1;
        a_cache = (seq_t *)realloc(a_cache, a_capacity * sizeof(seq_t));
    }

    for (idx_t i = a_len; i <= n; ++i)
        a_cache[i] = 3 * a_cache[i - 1] + (seq_t)1;

    a_len = n + 1;
    return a_cache[n];
}
