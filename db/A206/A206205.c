/*
 * A206205  Number of (n+1)X8 0..3 arrays with every 2X3 or 3X2 subblock
 *          having exactly one clockwise edge increases.
 * https://oeis.org/A206205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-3) for n>17; initial values for n = 1..17 are
 *            given below
 * Range:     u8 none, u16 none, u32 n <= 16, u64 n <= 45, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* For size_t, if needed */

seq_t a(idx_t n)
{
    /* Initial values a(1) .. a(17) */
    static const seq_t init[17] = {
        (seq_t)468968,   /* a(1)  */
        (seq_t)192808,   /* a(2)  */
        (seq_t)145208,   /* a(3)  */
        (seq_t)337896,   /* a(4)  */
        (seq_t)713408,   /* a(5)  */
        (seq_t)1080648,  /* a(6)  */
        (seq_t)3270240,  /* a(7)  */
        (seq_t)7008160,  /* a(8)  */
        (seq_t)10936160, /* a(9)  */
        (seq_t)33660800, /* a(10) */
        (seq_t)72736960, /* a(11) */
        (seq_t)113216800,/* a(12) */
        (seq_t)339966080,/* a(13) */
        (seq_t)729156480,/* a(14) */
        (seq_t)1132733728,/* a(15) */
        (seq_t)3399739520,/* a(16) */
        (seq_t)7291568000 /* a(17) */
    };

    if (n <= 17) return init[n - 1];

    seq_t base_val;
    idx_t k;

    /* Determine the congruence class modulo 3 and the number of factor 10s to apply */
    if ((n - 15) % 3 == 0) {
        base_val = init[14]; /* a(15) */
        k = (n - 15) / 3;
    } else if ((n - 16) % 3 == 0) {
        base_val = init[15]; /* a(16) */
        k = (n - 16) / 3;
    } else {
        base_val = init[16]; /* a(17) */
        k = (n - 17) / 3;
    }

    seq_t result = base_val;
    for (idx_t i = 0; i < k; ++i) {
        result *= (seq_t)10;
    }
    return result;
}
