/*
 * A159548  Positive numbers y such that y^2 is of the form x^2+(x+199)^2 with
 *          integer x.
 * https://oeis.org/A159548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n > 6; initial values: a(1)=181,
 *            a(2)=199, a(3)=221, a(4)=865, a(5)=995, a(6)=1145
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* zero or negative not part of OEIS offset, return 0 */
    if (n == 0)
        return 0;

    /* fixed initial values, stored in a static array */
    static const seq_t init[6] = {
        (seq_t)181, (seq_t)199, (seq_t)221,
        (seq_t)865, (seq_t)995, (seq_t)1145
    };

    /* return directly for the first six terms */
    if (n <= 6)
        return init[n - 1];

    /* buffer to hold the last 6 computed terms; use
       a rolling array to avoid allocating large memory. */
    seq_t buf[6];
    for (int i = 0; i < 6; ++i)
        buf[i] = init[i];

    /* iterate from the 7th term up to the requested term */
    for (idx_t i = 7; i <= n; ++i) {
        /* indices for a(i-3) and a(i-6) in the rolling buffer */
        idx_t idx_a = (i - 3 - 1) % 6;   /* a(i-3) */
        idx_t idx_b = (i - 6 - 1) % 6;   /* a(i-6) */

        /* recurrence: a(i) = 6*a(i-3) - a(i-6) */
        seq_t val = 6 * buf[idx_a] - buf[idx_b];

        /* overwrite the oldest value (a(i-6)) with a(i) */
        buf[(i - 1) % 6] = val;
    }

    /* the requested term is now in the rolling buffer */
    return buf[(n - 1) % 6];
}
