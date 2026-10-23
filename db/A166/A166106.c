/*
 * A166106  a(n) = a(n-1) + a(n-2) + F(n), with a(0) = 0, a(1) = 1, a(2) =
 *          a(1) + a(0), a(3) = a(2) + a(1), a(4) = a(3) + a(2) + 2.
 * https://oeis.org/A166106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - a(n-4) for n > 6 Initial
 *            values: a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=5, a(5)=12, a(6)=25
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 85, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t base[7] = {0, 1, 1, 2, 5, 12, 25};
    if (n <= 6) return base[n];

    seq_t a0 = base[3];  /* a[n-4] for the first iteration (n=7) */
    seq_t a1 = base[4];  /* a[n-3] */
    seq_t a2 = base[5];  /* a[n-2] */
    seq_t a3 = base[6];  /* a[n-1] */

    seq_t curr;
    for (idx_t i = 7; i <= n; ++i) {
        curr = a3 * 2 + a2 - a1 * 2 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = curr;
    }
    return a3;
}
