/*
 * A231064  Coins left after packing X patterns into an n X n array of coins.
 * https://oeis.org/A231064
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6) (n ≥ 18) Initial values: a(2) =
 *            4, a(3) = 4, a(4) = 11, a(5) = 15, a(6) = 16, a(7) = 24, a(8) =
 *            24, a(9) = 31, a(10) = 35, a(11) = 41, a(12) = 44, a(13) = 49,
 *            a(14) = 51, a(15) = 55, a(16) = 56, a(17) = 64 (The recurrence
 *            follows from the rational generating functi...
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[18] = {
    [0] = 0,
    [1] = 0,
    [2] = 4,
    [3] = 4,
    [4] = 11,
    [5] = 15,
    [6] = 16,
    [7] = 24,
    [8] = 24,
    [9] = 31,
    [10] = 35,
    [11] = 41,
    [12] = 44,
    [13] = 49,
    [14] = 51,
    [15] = 55,
    [16] = 56,
    [17] = 64
};

seq_t a(idx_t n)
{
    /* Use the pre‑computed values directly for the first 18 indices. */
    if (n <= 17)
        return init[n];

    /* Maintain the last six values needed for the recurrence. */
    seq_t a0 = init[12];  /* a(n-6)  */
    seq_t a1 = init[13];  /* a(n-5)  */
    seq_t a2 = init[14];  /* a(n-4)  */
    seq_t a3 = init[15];  /* a(n-3)  */
    seq_t a4 = init[16];  /* a(n-2)  */
    seq_t a5 = init[17];  /* a(n-1)  */

    /* Compute iteratively up to the requested n. */
    for (idx_t i = 18; i <= n; ++i) {
        seq_t an = a5 + a1 - a0;  /* a(i) = a(i-1) + a(i-5) - a(i-6) */
        /* Shift the window one step forward. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }

    return a5;  /* a(n) after the loop ends. */
}
