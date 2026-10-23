/*
 * A121458  Expansion of (1+x-2*x^2)/(1-21*x^2-7*x^3).
 * https://oeis.org/A121458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=19, a(n)=21*a(n-2)+7*a(n-3) for n>=3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 19;

    seq_t a0 = 1;  /* a(n-3) */
    seq_t a1 = 1;  /* a(n-2) */
    seq_t a2 = 19; /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t new_val = 21 * a1 + 7 * a0; /* a(n) = 21*a(n-2) + 7*a(n-3) */
        a0 = a1;
        a1 = a2;
        a2 = new_val;
    }
    return a2;
}
