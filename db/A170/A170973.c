/*
 * A170973  Expansion of Product_{i=0..m-1} (1 + x^(4*i+1)) for m = 10.
 * https://oeis.org/A170973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = number of subsets of {1,5,9,13,17,21,25,29,33,37} summing
 *            to n
 * Range:     u8 n <= 190, u16 n <= 190, u32 n <= 190, u64 n <= 190,
 *            u128 n <= 190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* maximum sum of all exponents: 1+5+9+13+17+21+25+29+33+37 = 190 */
    enum { MAX_DEGREE = 190 };
    enum { NUM_WEIGHTS = 10 };

    static seq_t cache[MAX_DEGREE + 1] = {0};
    static bool ready = false;

    if (!ready) {
        cache[0] = (seq_t)1;
        const idx_t weights[NUM_WEIGHTS] = {1,5,9,13,17,21,25,29,33,37};
        for (int wi = 0; wi < NUM_WEIGHTS; ++wi) {
            idx_t w = weights[wi];
            for (idx_t i = MAX_DEGREE; i >= w; --i) {
                cache[i] = cache[i] + cache[i - w];
            }
        }
        ready = true;
    }

    if (n > (idx_t)MAX_DEGREE)
        return (seq_t)0;
    return cache[(idx_t)n];
}
