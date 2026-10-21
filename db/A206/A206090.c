/*
 * A206090  Number of (n+1) X 5 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two counterclockwise and two clockwise edge
 *          increases.
 * https://oeis.org/A206090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 34 * a(n-3) for n > 7 Initial values: a(1)=3564,
 *            a(2)=5952, a(3)=13344, a(4)=48816, a(5)=131904, a(6)=444480,
 *            a(7)=1645920
 * Range:     u8 none, u16 n <= 4, u32 n <= 13, u64 n <= 32, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t init_vals[7] = {
    (seq_t)3564,
    (seq_t)5952,
    (seq_t)13344,
    (seq_t)48816,
    (seq_t)131904,
    (seq_t)444480,
    (seq_t)1645920
};

seq_t a(idx_t n)
{
    /* The OEIS offset is 1.  For n < 1 the value is undefined; return 0. */
    if (n == 0) return (seq_t)0;

    /* Direct lookup for the seven initial values. */
    if (n <= 7) return init_vals[n - 1];

    /* For n > 7, reduce n by multiples of 3 until it falls into the
       range 5 .. 7, because the recurrence uses a(n) = 34 * a(n-3). */
    idx_t t = (n - 5) / 3;          /* number of recurrence applications */
    idx_t m = n - 3 * t;            /* reduced index, 5 <= m <= 7 */

    seq_t result = init_vals[m - 1];

    /* Multiply by 34^t.  All intermediate results stay within a
       range that fits in seq_t for any n whose a(n) fits in seq_t. */
    for (idx_t i = 0; i < t; ++i)
        result *= (seq_t)34;

    return result;
}
