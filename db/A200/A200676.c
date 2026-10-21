/*
 * A200676  Expansion of g.f. -(3*x^2-5*x+1)/(x^3-3*x^2+5*x-1).
 * https://oeis.org/A200676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 33, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0, a(2)=0
   a(n)=5*a(n-1)-3*a(n-2)+a(n-3) for n≥3
   This follows from the rational generating function
   -(3*x^2-5*x+1)/(x^3-3*x^2+5*x-1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;

    seq_t prev3 = (seq_t)1;  /* a(0) */
    seq_t prev2 = (seq_t)0;  /* a(1) */
    seq_t prev1 = (seq_t)0;  /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = 5 * prev1 - 3 * prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
