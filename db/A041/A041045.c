/*
 * A041045  Denominators of continued fraction convergents to sqrt(28).
 * https://oeis.org/A041045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041045: Denominators of continued fraction convergents to sqrt(28)
   Recurrence: a(n) = 254*a(n-4) - a(n-8),  n ≥ 8
   Initial values: a(0)=1, a(1)=3, a(2)=7, a(3)=24, a(4)=247, a(5)=765,
                   a(6)=1777, a(7)=6096
*/

seq_t a(idx_t n)
{
    /* initial values for n = 0 … 7 */
    static const seq_t init[8] = {
        1,   /* a(0) */
        3,   /* a(1) */
        7,   /* a(2) */
        24,  /* a(3) */
        247, /* a(4) */
        765, /* a(5) */
        1777,/* a(6) */
        6096 /* a(7) */
    };

    if (n < 8)
        return init[n];

    /* circular buffer of length 8 that keeps the last 8 terms.
       Index i in the buffer corresponds to a(i).  */
    idx_t i;
    seq_t buf[8];
    for (i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (i = 8; i <= n; ++i) {
        idx_t idx1 = (i - 4) & 7U;   /* (i-4) mod 8 */
        idx_t idx2 = (i - 8) & 7U;   /* (i-8) mod 8 */
        buf[i & 7U] = 254 * buf[idx1] - buf[idx2];
    }

    return buf[n & 7U];
}
