/*
 * A186596  Number of (n+3)X9 0..3 arrays with every 4X4 subblock commuting
 *          with each horizontal and vertical neighbor 4X4 subblock.
 * https://oeis.org/A186596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 3*a(n-4) - 6*a(n-5) + 3*a(n-6) +
 *            3*a(n-8) - 3*a(n-9) for n > 16 Initial values: a(1) = 53301263,
 *            a(2) = 884466, a(3) = 1824789, a(4) = 5654805, a(5) = 12478581,
 *            a(6) = 16898709, a(7) = 17002221, a(8) = 17171973, a(9) =
 *            17632173, a(10) = 19933269, a(11) = 23358789, a(12)...
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined but safe for n<1 */

    /* Known initial 16 terms */
    static const seq_t init[16] = {
        (seq_t)53301263UL,
        (seq_t)884466UL,
        (seq_t)1824789UL,
        (seq_t)5654805UL,
        (seq_t)12478581UL,
        (seq_t)16898709UL,
        (seq_t)17002221UL,
        (seq_t)17171973UL,
        (seq_t)17632173UL,
        (seq_t)19933269UL,
        (seq_t)23358789UL,
        (seq_t)28260813UL,
        (seq_t)48277797UL,
        (seq_t)77435613UL,
        (seq_t)110277237UL,
        (seq_t)176424504UL
    };

    /* Return a known term directly */
    if (n <= 16) return init[n - 1];

    /* Buffer holding the last 9 values of the sequence
       buf[0] = a(k-9), buf[1] = a(k-8), ..., buf[8] = a(k-1)   */
    seq_t buf[9];
    size_t i;
    /* Initialize buffer with a(1) .. a(9) */
    for (i = 0; i < 9; ++i) buf[i] = init[i];

    /* Populate buffer up to a(16) using the known values */
    for (idx_t k = 10; k <= 16; ++k) {
        for (i = 0; i < 8; ++i) buf[i] = buf[i + 1];
        buf[8] = init[k - 1];
    }

    /* Compute terms from a(17) up to a(n) using the recurrence */
    for (idx_t k = 17; k <= n; ++k) {
        seq_t val =
            2 * buf[8]               /* a(k-1) */
            - buf[7]                 /* a(k-2) */
            + 3 * buf[5]             /* a(k-4) */
            - 6 * buf[4]             /* a(k-5) */
            + 3 * buf[3]             /* a(k-6) */
            + 3 * buf[1]             /* a(k-8) */
            - 3 * buf[0];            /* a(k-9) */

        for (i = 0; i < 8; ++i) buf[i] = buf[i + 1];
        buf[8] = val;               /* new a(k) becomes last element */
    }

    return buf[8];
}
