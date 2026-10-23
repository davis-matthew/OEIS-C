/*
 * A033304  Expansion of (2 + 2*x - 3*x^2) / (1 - 2*x - x^2 + x^3).
 * https://oeis.org/A033304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3), with a(0)=2, a(1)=6, a(2)=11
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 53, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 2;
    if (n == 1) return 6;
    if (n == 2) return 11;

    seq_t a0 = 2;      // a(n-3)
    seq_t a1 = 6;      // a(n-2)
    seq_t a2 = 11;     // a(n-1)
    seq_t ai;          // a(n)

    for (idx_t i = 3; i <= n; ++i) {
        ai = 2 * a2;   // 2*a(n-1)
        ai += a1;      // + a(n-2)
        ai -= a0;      // - a(n-3)
        a0 = a1;       // shift window
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
