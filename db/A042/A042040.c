/*
 * A042040  Numerators of continued fraction convergents to sqrt(544).
 * https://oeis.org/A042040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 4898*a(n-4) - a(n-8); Initial values: a(0)=23, a(1)=70,
 *            a(2)=793, a(3)=2449, a(4)=113447, a(5)=342790, a(6)=3884137,
 *            a(7)=11995201
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first 8 terms directly */
    if (n < 8)
    {
        static const seq_t init[8] = {
            23,       /* a(0) */
            70,       /* a(1) */
            793,      /* a(2) */
            2449,     /* a(3) */
            113447,   /* a(4) */
            342790,   /* a(5) */
            3884137,  /* a(6) */
            11995201  /* a(7) */
        };
        return init[n];
    }

    /* Circular buffer holding the last 8 computed values */
    seq_t buf[8] = {
        23,       /* a(0) */
        70,       /* a(1) */
        793,      /* a(2) */
        2449,     /* a(3) */
        113447,   /* a(4) */
        342790,   /* a(5) */
        3884137,  /* a(6) */
        11995201  /* a(7) */
    };

    for (idx_t i = 8; i <= n; ++i)
    {
        seq_t next = (seq_t)4898 * buf[(i - 4) & 7] - buf[(i - 8) & 7];
        buf[i & 7] = next;
    }

    return buf[n & 7];
}
