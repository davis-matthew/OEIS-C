/*
 * A123888  Expansion of g.f.: x/((1-x^2)^3 -1+x).
 * https://oeis.org/A123888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-3) + a(n-5) (Initial values) a(0)=1,
 *            a(1)=3, a(2)=9, a(3)=24, a(4)=63
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t init[5] = {
    (seq_t)1,   /* a(0) */
    (seq_t)3,   /* a(1) */
    (seq_t)9,   /* a(2) */
    (seq_t)24,  /* a(3) */
    (seq_t)63   /* a(4) */
};

seq_t a(idx_t n)
{
    if (n < 5) {
        return init[n];
    }

    /* Use a sliding window of the last five terms */
    seq_t x0 = init[0];
    seq_t x1 = init[1];
    seq_t x2 = init[2];
    seq_t x3 = init[3];
    seq_t x4 = init[4];
    seq_t val = 0;

    for (idx_t i = 5; i <= n; ++i) {
        /* Recurrence: a(i) = 3*a(i-1) - 3*a(i-3) + a(i-5) */
        val = 3 * x4 - 3 * x2 + x0;

        /* Shift the window forward */
        x0 = x1;
        x1 = x2;
        x2 = x3;
        x3 = x4;
        x4 = val;
    }

    return val;
}
