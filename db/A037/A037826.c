/*
 * A037826  Number of i such that d(i) >= d(i-1), where Sum_{i=0..m} d(i)*3^i
 *          is the base-3 representation of n.
 * https://oeis.org/A037826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = #{ i | d(i) ≥ d(i-1) } where n = Σ d(i)·3^i is the base‑3
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* When n < 1 the offset does not apply – return 0. */
    if (n == (idx_t)0) return (seq_t)0;

    /* Work with a copy of n to avoid modifying the argument. */
    idx_t x = n;

    /* Base‑3 digits of an n up to 64‑bit fit in < 41 digits. */
    idx_t digits[64];
    idx_t count = 0;
    while (x > (idx_t)0) {
        digits[count++] = x % (idx_t)3;
        x /= (idx_t)3;
    }

    /* Count positions i (1 ≤ i < count) with d(i) ≥ d(i-1). */
    seq_t result = (seq_t)0;
    for (idx_t i = (idx_t)1; i < count; ++i) {
        if (digits[i] >= digits[i-1]) {
            result++;
        }
    }
    return result;
}
