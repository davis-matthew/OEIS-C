/*
 * A177151  a(n) = least k such that 1 + 1/4 + 1/9 + ... + 1/k^2 exceeds
 *          (Pi^2)*(n-1)/(6*n).
 * https://oeis.org/A177151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that 1 + 1/4 + 1/9 + ... + 1/k² exceeds
 *            (π²)*(n-1)/(6*n)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pi²/6 as a high‑precision double constant. */
    const double pi2_over_6 = 1.6449340668482264365;

    /* Compute the target threshold for this n. */
    double target = pi2_over_6 * ((double)(n - 1) / (double)n);

    double sum = 0.0;
    idx_t k = 1;

    while (1) {
        double inv = 1.0 / ((double)k * (double)k);
        sum += inv;
        if (sum > target)
            break;
        ++k;
    }

    return (seq_t)k;
}
