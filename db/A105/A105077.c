/*
 * A105077  Expansion of g.f. -(x^3+5x+5)/((x^2-x+1)*(x+1)^2).
 * https://oeis.org/A105077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = -5, a(1) = 0, a(2) = 0, a(3) = 4,
   a(n) = -(a(n-1) + a(n-3) + a(n-4)) for n ≥ 4. */
seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)-5;
    if (n == 1U) return (seq_t)0;
    if (n == 2U) return (seq_t)0;
    if (n == 3U) return (seq_t)4;

    seq_t a0 = (seq_t)-5;  /* stores a(k-4)   */
    seq_t a1 = (seq_t)0;   /* stores a(k-3)   */
    seq_t a2 = (seq_t)0;   /* stores a(k-2)   */
    seq_t a3 = (seq_t)4;   /* stores a(k-1)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = -(a3 + a1 + a0);  /* a(k) = −(a(k-1)+a(k-3)+a(k-4)) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
