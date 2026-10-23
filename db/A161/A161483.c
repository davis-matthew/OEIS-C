/*
 * A161483  Positive numbers y such that y^2 is of the form x^2+(x+151)^2 with
 *          integer x.
 * https://oeis.org/A161483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n > 6; Initial values: a(1)=109,
 *            a(2)=151, a(3)=265, a(4)=389, a(5)=755, a(6)=1481
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fixed initial terms */
    static const seq_t init[6] = {(seq_t)109, (seq_t)151, (seq_t)265,
                                  (seq_t)389, (seq_t)755, (seq_t)1481};

    /* For indices 1..6 we can return directly. */
    if (n <= 6u)
        return init[n - 1];

    /* Maintain the last six computed values.
       The array is indexed as:
       buf[0] = a(i-6), buf[1] = a(i-5), …, buf[5] = a(i-1). */
    seq_t buf[6] = {init[0], init[1], init[2], init[3], init[4], init[5]};

    /* Iteratively compute a(i) for i from 7 up to n. */
    for (idx_t i = 7u; i <= n; ++i) {
        /* a(i) = 6*a(i-3) - a(i-6)  →  6*buf[3] - buf[0] */
        seq_t next = buf[3] * (seq_t)6 - buf[0];

        /* Shift the window: discard the oldest value and insert the new one. */
        for (int j = 0; j < 5; ++j)
            buf[j] = buf[j + 1];
        buf[5] = next;
    }

    /* After the loop buf[5] holds a(n). */
    return buf[5];
}
