/*
 * A082962  Numerators of continued fraction convergents to (sqrt(37)-4)/3.
 * https://oeis.org/A082962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 12*a(n-3) + a(n-6) initial values: a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=7, a(4)=9, a(5)=25
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 54, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* buffer storing the last six values of the sequence */
    static seq_t val[6];
    static int  initialized = 0;   /* whether initial values are set   */
    static idx_t last = 5;         /* index of the largest n already
                                      computed (at least 5)            */

    /* one‑time initialization of the base cases */
    if (!initialized) {
        val[0] = 0;
        val[1] = 1;
        val[2] = 2;
        val[3] = 7;
        val[4] = 9;
        val[5] = 25;
        initialized = 1;
    }

    /* for n < 6 the value is already in the buffer            */
    if (n < 6)
        return val[n];

    /* compute new values up to n if they are not already known */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            /* recurrence a(i) = 12*a(i-3) + a(i-6)                */
            val[i % 6] = 12 * val[(i - 3) % 6] + val[(i - 6) % 6];
        }
        last = n;
    }

    return val[n % 6];
}
