/*
 * A002525  Number of permutations according to distance.
 * https://oeis.org/A002525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-3) - a(n-5), with a(n) = 0 for n < 0,
 *            a(0) = 0, a(1) = 1 (from G.f.: x/(1 - 2*x - 2*x^3 + x^5))
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x5 = 0, x4 = 0, x3 = 0, x2 = 0, x1 = 0; /* a(i-5) .. a(i-1) */
    seq_t cur = 0;
    idx_t i;
    if (n == 0) return 0;
    if (n == 1) return 1;
    /* state for i = 2: a(-3)=a(-2)=a(-1)=0, a(0)=0, a(1)=1 */
    x5 = 0; x4 = 0; x3 = 0; x2 = 0; x1 = 1;
    for (i = 2; i <= n; i++) {
        cur = x1 + x1 + x3 + x3;
        cur -= x5;
        x5 = x4;
        x4 = x3;
        x3 = x2;
        x2 = x1;
        x1 = cur;
    }
    return cur;
}
