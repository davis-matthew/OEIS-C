/*
 * A046672  Expansion of 1/(1-2*x-3*x^2+2*x^3).
 * https://oeis.org/A046672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 2*a(n-3) initial values: a(0) = 1,
 *            a(1) = 2, a(2) = 7
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 7;

    seq_t a0 = 1, a1 = 2, a2 = 7, next;
    for (idx_t i = 3; i <= n; ++i) {
        next = (seq_t)2 * a2 + (seq_t)3 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
