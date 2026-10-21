/*
 * A206629  Number of (n+1) X 3 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having no more than four equal edges, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A206629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) + 68*a(n-2) + 132*a(n-3) + 8*a(n-4) + 48*a(n-5)
 *            Initial values: a(1)=121, a(2)=3232, a(3)=86392, a(4)=2309200,
 *            a(5)=61723264
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t init[5] = {
    (seq_t)121ULL,
    (seq_t)3232ULL,
    (seq_t)86392ULL,
    (seq_t)2309200ULL,
    (seq_t)61723264ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n<1, return 0 safely */

    if (n <= 5) return init[n-1];

    /* keep the last five computed values */
    seq_t a1 = init[0];          /* a(n-5) */
    seq_t a2 = init[1];          /* a(n-4) */
    seq_t a3 = init[2];          /* a(n-3) */
    seq_t a4 = init[3];          /* a(n-2) */
    seq_t a5 = init[4];          /* a(n-1) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = (seq_t)24 * a5
                    + (seq_t)68 * a4
                    + (seq_t)132 * a3
                    + (seq_t)8  * a2
                    + (seq_t)48 * a1;
        /* shift values for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
