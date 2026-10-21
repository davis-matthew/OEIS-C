/*
 * A052537  Expansion of (1-x)/(1-x-2*x^3).
 * https://oeis.org/A052537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n‑1) + 2*a(n‑3), a(0)=1, a(1)=0, a(2)=0
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 44, u64 n <= 86, u128 n <= 170,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_N 10000             /*  maximum index we expect to handle  */
static seq_t cache[MAX_N + 1];
static idx_t cached_len = 0;     /*  number of terms already computed  */

seq_t a(idx_t n)
{
    /*  Initialise the first three terms on first use  */
    if (cached_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cached_len = 3;
    }

    /*  Return a value we have already computed  */
    if (n < cached_len)
        return cache[n];

    /*  Compute all missing terms up to n inclusively  */
    for (idx_t i = cached_len; i <= n; ++i) {
        /*  a(i) = a(i-1) + 2*a(i-3)  */
        cache[i] = cache[i - 1] + ((seq_t)2 * cache[i - 3]);
    }

    cached_len = n + 1;
    return cache[n];
}
