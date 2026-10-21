/*
 * A063370  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          97 ).
 * https://oeis.org/A063370
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-4) + a(n-6) – a(n-10) for n > 13 with initial values
 *            a(2)=–1, a(3)=345, a(4)=736, a(5)=1128, a(6)=1520, a(7)=1912,
 *            a(8)=2304, a(9)=2696, a(10)=3088, a(11)=3480, a(12)=3872,
 *            a(13)=4262. a(n) is defined for all n ≥ 2
 * Range:     i8 n <= 2, i16 n <= 85, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; the sequence is only
       defined for n ≥ 2. */
    if (n < 2) return 0;

    /* Base values for indices 0 … 13.  Indices 0 and 1 are unused
       and set to 0. */
    static const seq_t base[14] = {
        0,      /* a(0) not used */
        0,      /* a(1) not used */
       -1,     /* a(2) */
        345,   /* a(3) */
        736,   /* a(4) */
       1128,   /* a(5) */
       1520,   /* a(6) */
       1912,   /* a(7) */
       2304,   /* a(8) */
       2696,   /* a(9) */
       3088,   /* a(10) */
       3480,   /* a(11) */
       3872,   /* a(12) */
       4262    /* a(13) */
    };

    /* For n ≤ 13 we can return the stored value directly. */
    if (n <= 13) {
        return base[(int)n];
    }

    /* Use a circular buffer of size 11 to hold the last 11 values,
       which is enough for the recurrence.  We compute the values
       iteratively from 14 up to the requested n.  The buffer
       indices are modulo 11. */
    seq_t buf[11];
    for (idx_t i = 2; i <= 13; ++i) {
        buf[(int)(i % 11)] = base[(int)i];
    }

    for (idx_t i = 14; i <= n; ++i) {
        int pos_cur   = (int)(i % 11);
        int pos4      = (int)((i - 4) % 11);
        int pos6      = (int)((i - 6) % 11);
        int pos10     = (int)((i - 10) % 11);
        buf[pos_cur] = buf[pos4] + buf[pos6] - buf[pos10];
    }

    return buf[(int)(n % 11)];
}
