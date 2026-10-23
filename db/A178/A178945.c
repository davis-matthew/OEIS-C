/*
 * A178945  Expansion of x*(1-x)^2/( (1-2*x^2)*(1-2*x)^2).
 * https://oeis.org/A178945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 8*a(n-3) + 8*a(n-4); Initial
 *            values: a(1)=1, a(2)=2, a(3)=7, a(4)=16
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)7;
    if (n == 4) return (seq_t)16;

    seq_t a1 = (seq_t)1;   /* a(n-4) */
    seq_t a2 = (seq_t)2;   /* a(n-3) */
    seq_t a3 = (seq_t)7;   /* a(n-2) */
    seq_t a4 = (seq_t)16;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t new_a = (seq_t)4 * a4 - (seq_t)2 * a3 - (seq_t)8 * a2 + (seq_t)8 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = new_a;
    }

    return a4;
}
