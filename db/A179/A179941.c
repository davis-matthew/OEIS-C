/*
 * A179941  Number of times n appears in a 100 X 100 multiplication table.
 * https://oeis.org/A179941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, less
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of pairs (i, j) with 1 ≤ i, j ≤ 100 and i*j = n */
#include <stdbool.h>

static seq_t table[10001]; /* indices 0..10000, ignore 0 */
static bool initialized = false;

seq_t a(idx_t n)
{
    if (n > 10000) {
        return (seq_t)0;
    }

    if (!initialized) {
        /* Precompute all products i*j for 1..100 */
        for (idx_t i = 1; i <= 100; ++i) {
            for (idx_t j = 1; j <= 100; ++j) {
                idx_t prod = i * j; /* prod ≤ 10000 */
                ++table[prod];
            }
        }
        initialized = true;
    }

    return table[n];
}
