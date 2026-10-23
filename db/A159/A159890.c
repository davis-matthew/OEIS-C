/*
 * A159890  Positive numbers y such that y^2 is of the form x^2+(x+439)^2 with
 *          integer x.
 * https://oeis.org/A159890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n>6; a(1)=401, a(2)=439, a(3)=485,
 *            a(4)=1921, a(5)=2195, a(6)=2509
 * Range:     u8 none, u16 n <= 10, u32 n <= 30, u64 n <= 66, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initialise the first six values (index 0 unused) */
    const seq_t init[7] = {
        0,      /* unused */
        401,    /* a(1) */
        439,    /* a(2) */
        485,    /* a(3) */
        1921,   /* a(4) */
        2195,   /* a(5) */
        2509    /* a(6) */
    };

    if (n <= 6)
        return init[n];

    /* buffer holding the last six computed terms.
       buf[0] = a(k-5), buf[1] = a(k-4), ..., buf[5] = a(k)  after each iteration. */
    seq_t buf[6] = {init[1], init[2], init[3], init[4], init[5], init[6]};

    for (idx_t i = 7; i <= n; ++i) {
        /* a(i) = 6*a(i-3) - a(i-6) */
        seq_t next = 6 * buf[3] - buf[0];  /* buf[3] = a(i-3), buf[0] = a(i-6) */

        /* Shift the buffer left and store the new term at the end */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }

    return buf[5];   /* a(n) */
}
