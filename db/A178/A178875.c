/*
 * A178875  Partial sums of round(4^n/9).
 * https://oeis.org/A178875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = floor((4*4^n - 3*n - 4) / 27) Derived recurrence: a(n) =
 *            5*a(n-1) - 4*a(n-2) + a(n-3) - 5*a(n-4) + 4*a(n-5), n > 4
 *            Initial values: a(0)=0, a(1)=0, a(2)=2, a(3)=9, a(4)=37
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values a(0)..a(4) */
    static const seq_t init[5] = {0, 0, 2, 9, 37};
    if (n < 5) return init[n];

    /* window of the last five terms: a(i-5)..a(i-1) */
    seq_t t0 = init[0];   /* a(i-5) */
    seq_t t1 = init[1];   /* a(i-4) */
    seq_t t2 = init[2];   /* a(i-3) */
    seq_t t3 = init[3];   /* a(i-2) */
    seq_t t4 = init[4];   /* a(i-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t val = 5*t4 - 4*t3 + t2 - 5*t1 + 4*t0;
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = val;
    }
    return t4;
}
