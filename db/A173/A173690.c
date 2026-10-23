/*
 * A173690  Partial sums of round(n^2/5).
 * https://oeis.org/A173690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 + n + 6)/30) Recurrence (used for
 *            efficient computation): a(0)=0, a(1)=0, a(2)=1, a(3)=3, a(4)=6
 *            for n>4: a(n) = a(n-5) + (n-2)^2 + 2
 * Range:     u8 n <= 15, u16 n <= 98, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values a(0) .. a(4) */
    const seq_t init[5] = {0, 0, 1, 3, 6};

    if (n < 5)
        return init[n];

    /* ring buffer holding last 5 values */
    seq_t buf[5];
    buf[0] = init[0];
    buf[1] = init[1];
    buf[2] = init[2];
    buf[3] = init[3];
    buf[4] = init[4];

    for (idx_t i = 5; i <= n; ++i) {
        idx_t idx = i % 5;                     /* position of a(i-5) */
        seq_t old = buf[idx];                  /* a(i-5)   */
        seq_t term = (seq_t)(i - 2) * (seq_t)(i - 2); /* (i-2)^2 */
        buf[idx] = old + term + 2;             /* a(i) = a(i-5)+(i-2)^2+2 */
    }

    return buf[n % 5];
}
