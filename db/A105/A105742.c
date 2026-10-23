/*
 * A105742  For n>2, a(n) > 0 is such that a(n-1)^2+4*a(n-2)*a(n) is a minimal
 *          square, a(1)=1,a(2)=9.
 * https://oeis.org/A105742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the unique positive integer such that * a(n-1)^2 +
 *            4*a(n-2)*a(n) is a perfect square, and * this square is the
 *            smallest possible. Initial values are * a(1)=1, a(2)=9
 * Range:     u8 n <= 3, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;        /* not part of the offset */
    if (n == 1) return 1;
    if (n == 2) return 9;

    seq_t prevprev = 1;               /* a(1) */
    seq_t prev     = 9;               /* a(2) */
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i) {
        /* Find minimal m > prev such that
         * (m^2 - prev^2) is divisible by (4*prevprev).
         * Then a(n) = (m^2 - prev^2) / (4*prevprev).        */
        seq_t m      = prev + 1;
        seq_t denom  = (seq_t)4 * prevprev;
        seq_t a_val  = 0;

        for (;;) {
            seq_t m2   = m * m;
            seq_t pr2  = prev * prev;
            seq_t diff = m2 - pr2;
            if (diff % denom == 0) {
                a_val = diff / denom;
                if (a_val > 0) break;
            }
            ++m;
        }
        cur = a_val;
        prevprev = prev;
        prev = cur;
    }
    return prev;   /* prev holds a(n) after the loop */
}
