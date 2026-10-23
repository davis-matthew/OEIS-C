/*
 * A117620  Start with x=4/3; repeatedly apply the map x -> (x^2) ceiling(x);
 *          sequence gives numerators of the resulting sequence of fractions.
 * https://oeis.org/A117620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(1)=4; for n>1 let x_k = a(k)/b(k) with x_1=4/3 and x_{k+1} =
 *            (x_k)^2 * ceil(x_k); then a(n) is the numerator of x_n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *num_cache = NULL;
static seq_t *den_cache = NULL;
static idx_t cached = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety; not used */

    /* Grow the cache if needed */
    if (cached < n) {
        idx_t new_size = n;

        /* Reallocate the arrays */
        seq_t *num_new = (seq_t *)realloc(num_cache, new_size * sizeof(seq_t));
        seq_t *den_new = (seq_t *)realloc(den_cache, new_size * sizeof(seq_t));
        if (!num_new || !den_new) abort();

        num_cache = num_new;
        den_cache = den_new;

        /* If this is the very first element, initialize 4/3 */
        if (cached == 0) {
            num_cache[0] = (seq_t)4;
            den_cache[0] = (seq_t)3;
            cached = 1;
        }

        /* Compute the remaining terms */
        for (idx_t i = cached; i < new_size; ++i) {
            seq_t prev_num = num_cache[i - 1];
            seq_t prev_den = den_cache[i - 1];

            /* ceil(prev_num / prev_den) */
            seq_t m = (prev_num + prev_den - (seq_t)1) / prev_den;

            /* raw numerator = prev_num^2 * m */
            seq_t raw_num = prev_num * prev_num;
            raw_num = raw_num * m;

            /* raw denominator = prev_den^2 */
            seq_t raw_den = prev_den * prev_den;

            /* Reduce */
            seq_t g = gcd(raw_num, raw_den);
            num_cache[i] = raw_num / g;
            den_cache[i] = raw_den / g;
        }
        cached = new_size;
    }

    return num_cache[n - 1];
}
