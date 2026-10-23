/*
 * A195571  Denominators a(n) of Pythagorean approximations b(n)/a(n) to 1/5.
 * https://oeis.org/A195571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A195571
    Recurrence:  a(n) = 101*a(n-3) + 101*a(n-6) - a(n-9)
    Initial terms:  a(1)=1, a(2)=40, a(3)=60, a(4)=99,
                    a(5)=4100, a(6)=6100, a(7)=10101,
                    a(8)=418140, a(9)=622160
*/
seq_t a(idx_t n)
{
    /* first nine terms (offset 1) */
    static const seq_t init[9] = {
        (seq_t)1,      /* a(1) */
        (seq_t)40,     /* a(2) */
        (seq_t)60,     /* a(3) */
        (seq_t)99,     /* a(4) */
        (seq_t)4100,   /* a(5) */
        (seq_t)6100,   /* a(6) */
        (seq_t)10101,  /* a(7) */
        (seq_t)418140, /* a(8) */
        (seq_t)622160  /* a(9) */
    };

    if (n <= 9)
        return init[(int)(n - 1)];

    /* sliding window for last 9 values: buf[0] = a(k-9), …, buf[8] = a(k-1) */
    seq_t buf[9];
    for (int i = 0; i < 9; ++i)
        buf[i] = init[i];

    seq_t t = 0;  /* will hold a(k) for the current k */
    for (idx_t k = 10; k <= n; ++k)
    {
        /* a(k) = 101*a(k-3) + 101*a(k-6) - a(k-9) */
        t = buf[6] * (seq_t)101;    /* 101*a(k-3) */
        t += buf[3] * (seq_t)101;   /* +101*a(k-6) */
        t -= buf[0];                /* -a(k-9) */

        /* shift buffer left and insert the new value */
        for (int i = 0; i < 8; ++i)
            buf[i] = buf[i + 1];
        buf[8] = t;
    }
    return t;
}
