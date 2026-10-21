/*
 * A209899  Floor of the expected number of empty cells in a random placement
 *          of 2n balls into n cells.
 * https://oeis.org/A209899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(1-1/n)^(2n))
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Guard for n = 0 (should not occur for OEIS offset 1). */
    if (n == 0) return (seq_t)0;

    long double nd = (long double)n;
    long double base = (long double)(n - 1) / nd;
    /* Compute n * base^(2*n) */
    long double val = nd * powl(base, 2.0L * nd);

    /* Add a small epsilon to guard against floating‑point round‑off on the
       lower side. The absolute error is far below 1e‑12 for n <= 10000. */
    const long double eps = 1e-12L;
    unsigned long long tmp = (unsigned long long)floorl(val + eps);

    return (seq_t)tmp;
}
