/*
 * A033517  Number of matchings in graph C_{5} X P_{n}.
 * https://oeis.org/A033517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the 8-term linear recurrence a(n) = 25*a(n-1) +
 *            76*a(n-2) - 209*a(n-3) - 159*a(n-4) + 119*a(n-5) + 40*a(n-6) -
 *            3*a(n-7) - a(n-8) with initial values: a(0)=1, a(1)=11,
 *            a(2)=342, a(3)=9213, a(4)=253880, a(5)=6974078, a(6)=191668283,
 *            a(7)=5267252351, a(8)=144751259054
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values up to a(8) */
    static const seq_t init_vals[9] = {
        (seq_t)1L,
        (seq_t)11L,
        (seq_t)342L,
        (seq_t)9213L,
        (seq_t)253880L,
        (seq_t)6974078L,
        (seq_t)191668283L,
        (seq_t)5267252351L,
        (seq_t)144751259054L
    };

    if (n == 0) return init_vals[0];
    if (n < 8)  return init_vals[n];

    /* ring buffer holding the last 9 terms: buf[0]=a(n-8)...buf[8]=a(n-1) */
    seq_t buf[9];
    for (int i = 0; i < 9; ++i) buf[i] = init_vals[i];

    for (idx_t i = 9; i <= n; ++i) {
        /* compute a(i) from the recurrence */
        seq_t pos = 0;
        pos += buf[8] * (seq_t)25L;   /* 25*a(i-1) */
        pos += buf[7] * (seq_t)76L;   /* 76*a(i-2) */
        pos += buf[4] * (seq_t)119L;  /* 119*a(i-5) */
        pos += buf[3] * (seq_t)40L;   /* 40*a(i-6) */

        seq_t neg = 0;
        neg += buf[6] * (seq_t)209L;  /* 209*a(i-3) */
        neg += buf[5] * (seq_t)159L;  /* 159*a(i-4) */
        neg += buf[2] * (seq_t)3L;    /* 3*a(i-7) */
        neg += buf[1];                /* a(i-8) */

        seq_t val = pos - neg;        /* positive result */

        /* shift buffer left and append new value */
        for (int k = 0; k < 8; ++k) buf[k] = buf[k + 1];
        buf[8] = val;
    }
    return buf[8];
}
