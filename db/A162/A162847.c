/*
 * A162847  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 33*a(n-1) + 33*a(n-2) - 561*a(n-3)  for n ≥ 3
   Initial values: a(0)=1, a(1)=35, a(2)=1190, a(3)=39865 */

#include <stddef.h>   /* for size_t */

static seq_t cache[10001];
static size_t max_generated = 0;

seq_t a(idx_t n)
{
    /* Ensure cache has initial 4 terms */
    if (max_generated == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)35;
        cache[2] = (seq_t)1190;
        cache[3] = (seq_t)39865;
        max_generated = 3; /* we have computed indices 0..3 */
    }

    /* If n is within the already computed range, just return it */
    if ((size_t)n <= max_generated) {
        return cache[(size_t)n];
    }

    /* Compute terms up to n */
    for (size_t i = max_generated + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)33 * cache[i-1] +
                   (seq_t)33 * cache[i-2] -
                   (seq_t)561 * cache[i-3];
    }
    max_generated = (size_t)n;
    return cache[(size_t)n];
}
