/*
 * A006318  Large Schröder numbers (or large Schroeder numbers, or big
 *          Schroeder numbers).
 * https://oeis.org/A006318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice, changed
 * Formula:   (n+1)*a(n) = 3*(2n-1)*a(n-1) - (n-2)*a(n-2), a(0) = 1, a(1) = 2.
 *            To avoid overflow, each previous term is split as a(k) = q*(n+1)
 *            + r, so the division by n+1 is done exactly on small remainders
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;

    seq_t p2 = 1;  /* a(m-2) */
    seq_t p1 = 2;  /* a(m-1) */

    for (idx_t m = 2; m <= n; m++) {
        seq_t d = (seq_t)(m + 1);
        idx_t c1 = 3 * (2 * m - 1);
        idx_t c2 = m - 2;

        seq_t q1 = p1 / d;
        unsigned long long r1 = (unsigned long long)(p1 % d);
        seq_t q2 = p2 / d;
        unsigned long long r2 = (unsigned long long)(p2 % d);

        seq_t res = q1 * (seq_t)c1 - q2 * (seq_t)c2;

        unsigned long long X = (unsigned long long)c1 * r1;
        unsigned long long Y = (unsigned long long)c2 * r2;
        if (X >= Y)
            res += (seq_t)((X - Y) / (unsigned long long)(m + 1));
        else
            res -= (seq_t)((Y - X) / (unsigned long long)(m + 1));

        p2 = p1;
        p1 = res;
    }

    return p1;
}
