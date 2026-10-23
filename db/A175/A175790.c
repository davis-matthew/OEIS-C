/*
 * A175790  Expansion of 1/((1 - x^3 - x^4)*(1 + x)).
 * https://oeis.org/A175790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + a(n-3) + 2*a(n-4) + a(n-5) (n >= 6) Initial
 *            values: a(0)=1, a(1)=-1, a(2)=1, a(3)=0, a(4)=1, a(5)=-1
 * Range:     i8 n <= 33, i16 n <= 61, i32 n <= 116, i64 n <= 228,
 *            i128 n <= 450, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* base values */
    static const seq_t base[6] = { 1, -1, 1, 0, 1, -1 };
    if (n <= 5) {
        return base[n];
    }

    seq_t buf[6];
    /* initialise the circular buffer with the first 6 terms */
    for (idx_t i = 0; i < 6; ++i) {
        buf[i] = base[i];
    }

    /* compute terms from 6 up to n */
    for (idx_t i = 6; i <= n; ++i) {
        /* recurrence: a(i) = -a(i-1) + a(i-3) + 2*a(i-4) + a(i-5) */
        seq_t val = -buf[(i - 1) % 6]          /* -a(i-1) */
                    + buf[(i - 3) % 6]         /* +a(i-3) */
                    + 2 * buf[(i - 4) % 6]     /* +2*a(i-4) */
                    + buf[(i - 5) % 6];        /* +a(i-5) */
        buf[i % 6] = val;
    }
    return buf[n % 6];
}
