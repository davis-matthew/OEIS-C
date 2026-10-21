/*
 * A213720  a(0)=0, a(1)=0; for n>1, a(n) = a(n-1) + (n+2)*a(n-2) + 1.
 * https://oeis.org/A213720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (n+2)*a(n-2) + 1; with a(0) = 0, a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 30, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 0 || n == 1)
        return 0;

    /* Use rolling variables to avoid storing the entire sequence. */
    seq_t prev2 = 0; /* a(n-2) */
    seq_t prev1 = 0; /* a(n-1) */
    seq_t current;

    /* Iterate from n = 2 up to the requested n. */
    for (idx_t i = 2; i <= n; ++i) {
        /* Compute a(i) = a(i-1) + (i+2)*a(i-2) + 1 */
        current = prev1 + ((seq_t)i + 2) * prev2 + 1;

        /* Shift the window for the next iteration. */
        prev2 = prev1;
        prev1 = current;
    }

    return prev1;
}
