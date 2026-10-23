/*
 * A033545  Uniform numbers retired by New York Yankees.
 * https://oeis.org/A033545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) is the nth uniform number retired by the New York Yankees
 *            (duplicates allowed)
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Precomputed list of retired uniform numbers (duplicates represent consecutive retirements). */
    static const seq_t vals[] = {
        1, 3, 4, 5, 7, 8, 8, 9, 10, 15,
        16, 23, 32, 37, 42, 42, 44, 49
    };
    const size_t len = sizeof(vals)/sizeof(vals[0]);

    if (n < 1) return 0;           /* offset is 1; values are undefined for n < 1 */
    if ((size_t)(n-1) < len) return vals[n-1];
    return 0;                       /* undefined for indices beyond available data */
}
