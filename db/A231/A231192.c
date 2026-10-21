/*
 * A231192  Number of (n+1) X (1+1) 0..2 arrays with no element equal to a
 *          strict majority of its horizontal, vertical and antidiagonal
 *          neighbors, with values 0..2 introduced in row major order.
 * https://oeis.org/A231192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 4*a(n-3) + 4*a(n-4) - 12*a(n-5) - 16*a(n-6).
 *            Initial values: a(1) = 9 a(2) = 67 a(3) = 538 a(4) = 4264 a(5) =
 *            33868 a(6) = 268936
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first six terms directly. */
    switch (n) {
        case 1: return (seq_t)9;
        case 2: return (seq_t)67;
        case 3: return (seq_t)538;
        case 4: return (seq_t)4264;
        case 5: return (seq_t)33868;
        case 6: return (seq_t)268936;
        default: break;
    }

    /* Use a sliding window of the last six values. */
    seq_t a1 = (seq_t)9;    /* a(n-5) */
    seq_t a2 = (seq_t)67;   /* a(n-4) */
    seq_t a3 = (seq_t)538;  /* a(n-3) */
    seq_t a4 = (seq_t)4264; /* a(n-2) */
    seq_t a5 = (seq_t)33868;/* a(n-1) */
    seq_t a6 = (seq_t)268936;/* a(n) */

    idx_t k;
    for (k = 7; k <= n; ++k) {
        seq_t an = (seq_t)8 * a6
                   - (seq_t)4 * a4
                   + (seq_t)4 * a3
                   - (seq_t)12 * a2
                   - (seq_t)16 * a1;

        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = an;
    }
    return a6;
}
