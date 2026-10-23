/*
 * A020865  Number of forests in Moebius ladder M_n.
 * https://oeis.org/A020865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4+√12)^n+(4-√12)^n+2^n-(5/2+½√17)^n-(5/2-½√17)^n-1
 *            Recurrence (for n ≥ 5): a(n) = 15 a(n-1) - 72 a(n-2) + 128
 *            a(n-3) - 80 a(n-4) + 16 a(n-5) + 6 Initial values: a(0) = 0,
 *            a(1) = 4, a(2) = 38, a(3) = 328, a(4) = 2686 The recurrence
 *            follows from the characteristic polynomial (x‑2)(x²‑8x+4)(...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0..4 */
    static const seq_t init[5] = {0, 4, 38, 328, 2686};

    if (n < 5u)
        return init[(size_t)n];

    /* Sliding window of the last five terms */
    seq_t w[5];
    w[0] = init[0];
    w[1] = init[1];
    w[2] = init[2];
    w[3] = init[3];
    w[4] = init[4];

    for (idx_t i = 5; i <= n; ++i) {
        /* Compute a(i) from the recurrence */
        seq_t term1 = 15 * w[4];
        term1 += 128 * w[2];
        term1 += 16 * w[0];
        term1 += 6;

        seq_t term2 = 72 * w[3];
        term2 += 80 * w[1];

        seq_t next = term1 - term2; /* guaranteed non‑negative in exact arithmetic */

        /* Shift the window for the next iteration */
        w[0] = w[1];
        w[1] = w[2];
        w[2] = w[3];
        w[3] = w[4];
        w[4] = next;
    }
    return w[4];
}
