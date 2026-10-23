/*
 * A187612  Number of 8-step one space for components leftwards or up, two
 *          space for components rightwards or down asymmetric quasi-bishop's
 *          tours (antidiagonal moves become knight moves) on an n X n board
 *          summed over all starting positions.
 * https://oeis.org/A187612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n > 16 (initial values:
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=474, a(7)=3780,
 *            a(8)=12946, a(9)=32869, a(10)=68308, a(11)=117760, a(12)=187311,
 *            a(13)=275272, a(14)=379035, a(15)=500919, a(16)=639115)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t a_init[16] = {
    (seq_t)0,   (seq_t)0,   (seq_t)0,   (seq_t)0,   (seq_t)0,
    (seq_t)474, (seq_t)3780, (seq_t)12946, (seq_t)32869, (seq_t)68308,
    (seq_t)117760, (seq_t)187311, (seq_t)275272, (seq_t)379035,
    (seq_t)500919, (seq_t)639115
};

seq_t a(idx_t n) {
    if (n <= 16 && n >= 1) {
        return a_init[n - 1];
    }

    /* Use a rolling window of the last three terms. */
    seq_t a_nm1 = a_init[15]; /* a(n-1) for the first iteration (n = 17) */
    seq_t a_nm2 = a_init[14]; /* a(n-2) */
    seq_t a_nm3 = a_init[13]; /* a(n-3) */

    idx_t k;
    for (k = 17; k <= n; ++k) {
        seq_t next = (seq_t)3 * a_nm1 - (seq_t)3 * a_nm2 + a_nm3;
        /* shift the window */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
