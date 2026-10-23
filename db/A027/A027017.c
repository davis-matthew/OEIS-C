/*
 * A027017  a(n) = T(2*n+1, n+2), T given by A027011.
 * https://oeis.org/A027017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 20, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function:
   a(n) = 7*a(n-1) - 19*a(n-2) + 26*a(n-3) - 19*a(n-4) + 7*a(n-5) - a(n-6)
   for n ≥ 8.  Initial values (n = 1 … 6) are 1, 7, 73, 373, 1404, 4506.
   The term a(7) is obtained from the same algebraic pattern but the
   constant term +1 (coming from the numerator of the generating function)
   must be added once.  The function works for any of the integer types
   seq_t and idx_t provided by the build. */
seq_t a(idx_t n)
{
    /* first six terms */
    static const seq_t init[6] = {
        (seq_t)1,
        (seq_t)7,
        (seq_t)73,
        (seq_t)373,
        (seq_t)1404,
        (seq_t)4506
    };

    if (n == 0)
        return (seq_t)0;          /* undefined, but harmless */

    if (n <= 6)
        return init[n - 1];

    /* buffer holds the last six values: a(k-5) … a(k) after each step */
    seq_t buf[6];
    for (idx_t i = 0; i < 6; ++i)
        buf[i] = init[i];

    for (idx_t k = 7; k <= n; ++k) {
        /* compute next value using the recurrence */
        seq_t pos = buf[5] * (seq_t)7        /* 7*a(k-1) */
                 + buf[3] * (seq_t)26        /* 26*a(k-3) */
                 + buf[1] * (seq_t)7;        /* 7*a(k-5) */

        seq_t neg = buf[4] * (seq_t)19       /* 19*a(k-2) */
                 + buf[2] * (seq_t)19        /* 19*a(k-4) */
                 + buf[0];                   /* a(k-6) */

        seq_t cur = pos - neg;

        /* for k == 7 the numerator contributes +1 */
        if (k == 7)
            cur += (seq_t)1;

        /* shift the circular buffer: drop a(k-6), keep a(k-5)…a(k) */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = cur;
    }

    return buf[5];
}
