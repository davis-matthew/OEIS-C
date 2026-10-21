/*
 * A061992  Number of ways to place 6 nonattacking queens on a 6 X n board.
 * https://oeis.org/A061992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 - 45*n^5 + 943*n^4 - 11755*n^3 + 91480*n^2 - 418390*n
 *            + 870920 (for n ≥ 17)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values for n < 17 are taken from the OEIS b-file */

#include <stddef.h>

static const seq_t pre[17] = {
    (seq_t)0,  (seq_t)0,  (seq_t)0,  (seq_t)0,  (seq_t)0,
    (seq_t)0,  (seq_t)4,  (seq_t)94, (seq_t)550, (seq_t)2292,
    (seq_t)7552, (seq_t)21362, (seq_t)52856, (seq_t)117694,
    (seq_t)241484, (seq_t)463038, (seq_t)838816
};

seq_t a(idx_t n)
{
    /* Return the known small values directly. */
    if (n < 17)
        return pre[(size_t)n];

    /* Compute the explicit polynomial for n ≥ 17. */
    seq_t nn = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n3  = n2 * nn;
    seq_t n4  = n3 * nn;
    seq_t n5  = n4 * nn;
    seq_t n6  = n5 * nn;

    seq_t value = n6;
    value -= (seq_t)45 * n5;
    value += (seq_t)943 * n4;
    value -= (seq_t)11755 * n3;
    value += (seq_t)91480 * n2;
    value -= (seq_t)418390 * nn;
    value += (seq_t)870920;

    return value;
}
