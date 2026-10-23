/*
 * A145102  a(0) = a(1) = 1. a(n+1) = floor(n*a(n)/a(n-1)), for n >= 1.
 * https://oeis.org/A145102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=1; a(n+1)=floor(n*a(n)/a(n-1)) for n>=1
 * Range:     u8 n <= 52, u16 n <= 892, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t cache[62];
static bool cache_filled = false;

seq_t a(idx_t n)
{
    if (n < 62) {
        if (!cache_filled) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)1;
            for (idx_t i = 2; i < 62; ++i) {
                cache[i] = ((seq_t)(i - 1) * cache[i - 1]) / cache[i - 2];
            }
            cache_filled = true;
        }
        return cache[n];
    }

    idx_t r = n % 6;
    if (r == 0) {
        seq_t term = (seq_t)(n / 6);
        term += ((seq_t)(n - 2) * (n - 3)) / 12;
        term -= (seq_t)1;
        return term;
    } else if (r == 1) {
        return (seq_t)n;
    } else if (r == 2 || r == 3) {
        return (seq_t)12;
    } else if (r == 4) {
        return (seq_t)(n - 1);
    } else { /* r == 5 */
        return ((seq_t)(n - 1) * (n - 2)) / 12;
    }
}
