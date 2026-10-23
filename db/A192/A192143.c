/*
 * A192143  0-sequence of reduction of hexagonal numbers sequence by x^2 ->
 *          x+1.
 * https://oeis.org/A192143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Linear recurrence for OEIS A192143
 * Derived from the rational generating function.
 * For n ≥ 8:
 *   a(n) = 4*a(n-1) - 3*a(n-2) - 5*a(n-3) + 5*a(n-4)
 *          + 3*a(n-5) - 2*a(n-6) - a(n-7)
 * Initial values:
 *   a(1)=1, a(2)=1, a(3)=16, a(4)=44, a(5)=134,
 *   a(6)=332, a(7)=787.
 */
seq_t a(idx_t n)
{
    /* initial known terms */
    static const seq_t init[7] = {1, 1, 16, 44, 134, 332, 787};

    if (n == 0) {          /* below offset – behavior unspecified */
        return (seq_t)0;
    }
    if (n <= 7) {
        return init[n-1];
    }

    seq_t buf[7];
    /* buf[0]..buf[6] correspond to a(n-7)..a(n-1) */
    for (idx_t i = 0; i < 7; ++i) {
        buf[i] = init[i];
    }

    for (idx_t k = 8; k <= n; ++k) {
        /* compute a(k) from the last seven values */
        seq_t pos = 4*buf[6] + 5*buf[3] + 3*buf[2];
        seq_t neg = 3*buf[5] + 5*buf[4] + 2*buf[1] + buf[0];
        seq_t val = pos - neg;

        /* shift buffer left by one */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = val;
    }

    return buf[6];
}
