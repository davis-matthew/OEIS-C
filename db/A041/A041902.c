/*
 * A041902  Numerators of continued fraction convergents to sqrt(473).
 * https://oeis.org/A041902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 174*a(n-4) - a(n-8) (n ≥ 8) Initial values: a(0)=21,
 *            a(1)=22, a(2)=65, a(3)=87, a(4)=3719, a(5)=3806, a(6)=11331,
 *            a(7)=15137
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* size_t */

static const seq_t init_vals[8] = {
    21, 22, 65, 87,
    3719, 3806, 11331, 15137
};

/* Computes the n-th term of sequence A041902 */
seq_t a(idx_t n)
{
    /* For indices smaller than the offset just return the constant */
    if (n < 8) return init_vals[(size_t)n];

    /* Sliding window of the last eight terms – enough for the recurrence */
    seq_t prev[8];
    for (size_t i = 0; i < 8; ++i) prev[i] = init_vals[i];

    seq_t cur = 0;
    for (size_t i = 8; i <= (size_t)n; ++i) {
        cur = (seq_t)174 * prev[4] - prev[0];

        /* shift the window left by one position */
        for (size_t j = 0; j < 7; ++j) prev[j] = prev[j + 1];
        prev[7] = cur;
    }
    return cur;
}
