/*
 * A129992  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2 + (x+127)^2 = y^2.
 * https://oeis.org/A129992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 254 for n > 6; a(1)=0, a(2)=17,
 *            a(3)=308, a(4)=381, a(5)=468, a(6)=2117
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 32, u64 n <= 68, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n<1 */

    /* ensure we have storage for indices 0..n */
    if (n + 1 > cache_cap) {
        if (cache) free(cache);
        cache = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory */
        cache_cap = n + 1;
    }

    /* base cases (indices 1..6) */
    if (n >= 1) cache[1] = (seq_t)(0);
    if (n >= 2) cache[2] = (seq_t)(17);
    if (n >= 3) cache[3] = (seq_t)(308);
    if (n >= 4) cache[4] = (seq_t)(381);
    if (n >= 5) cache[5] = (seq_t)(468);
    if (n >= 6) cache[6] = (seq_t)(2117);

    /* recurrence for n > 6 */
    for (idx_t i = 7; i <= n; ++i) {
        cache[i] = 6 * cache[i - 3] - cache[i - 6] + (seq_t)(254);
    }

    return cache[n];
}
