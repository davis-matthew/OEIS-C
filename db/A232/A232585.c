/*
 * A232585  Number of (n+1)X(4+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally or
 *          antidiagonally, with no adjacent elements equal.
 * https://oeis.org/A232585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 9*a(n-2) + 2*a(n-3) + a(n-4) + 2*a(n-5) for n
 *            ≥ 6, with initial values a(1) = 26, a(2) = 152, a(3) = 996, a(4)
 *            = 6406, a(5) = 41328
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Ensure n ≥ 1; if n == 0 the sequence is undefined, return 0 */
    if (n == 0) return 0;

    /* Initial terms a(1)..a(5) */
    static const seq_t init[5] = {26, 152, 996, 6406, 41328};

    if (n <= 5) return init[n - 1];

    /* Sliding window holding a(n-5)..a(n-1) */
    seq_t prev[5];
    for (int i = 0; i < 5; ++i) prev[i] = init[i];

    seq_t next = 0;
    for (idx_t i = 6; i <= n; ++i) {
        /* Compute a(i) from previous five values */
        next = 5 * prev[4] + 9 * prev[3] + 2 * prev[2] + prev[1] + 2 * prev[0];
        /* Shift window: drop a(i-5) and append a(i) */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = next;
    }
    return next;
}
