/*
 * A001241  Differences of reciprocals of unity.
 * https://oeis.org/A001241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 50*a(n-1) - 840*a(n-2) + 5760*a(n-3) - 13824*a(n-4),
 *            a(-2) = a(-1) = a(0) = 0, a(1) = 1 (from G.f. x /
 *            ((1-6*x)*(1-8*x)*(1-12*x)*(1-24*x)))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a1 = 1, a2 = 0, a3 = 0, a4 = 0;
    idx_t i;
    for (i = 2; i <= n; i++) {
        seq_t pos = a1 * 50 + a3 * 5760;
        seq_t neg = a2 * 840 + a4 * 13824;
        seq_t next = pos - neg;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = next;
    }
    return a1;
}
