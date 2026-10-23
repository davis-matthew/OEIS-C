/*
 * A101677  a(n) = a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *          a(n-6).
 * https://oeis.org/A101677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            a(n-6) with initial values a(0)=1, a(1)=1, a(2)=-1, a(3)=-2,
 *            a(4)=-2, a(5)=-2
 * Range:     i8 n <= 385, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values for n = 0 .. 5 */
    static const seq_t init[6] = {1, 1, -1, -2, -2, -2};

    if (n < 6)
        return init[n];

    /* Keep last six values: v[0] = a(k-6), …, v[5] = a(k-1) */
    seq_t v[6];
    for (int i = 0; i < 6; ++i)
        v[i] = init[i];

    for (idx_t k = 6; k <= n; ++k) {
        /* Recurrence: a(k) = 2*v[5] - 2*v[4] + 2*v[3] - 2*v[2] + 2*v[1] - v[0] */
        seq_t curr = (seq_t)2 * v[5] - (seq_t)2 * v[4] + (seq_t)2 * v[3]
                     - (seq_t)2 * v[2] + (seq_t)2 * v[1] - v[0];

        /* Shift the ring buffer forward */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = curr;
    }

    /* The desired value is now in v[5] */
    return v[5];
}
