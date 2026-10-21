/*
 * A216949  Expansion of g.f. (1-6*x+7*x^2)/(1-7*x+11*x^2-x^3).
 * https://oeis.org/A216949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=3;
   For n≥3, a(n) = 7*a(n-1) - 11*a(n-2) + a(n-3) */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 3;

    seq_t a0 = 1, a1 = 1, a2 = 3;
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        next = 7 * a2 - 11 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
