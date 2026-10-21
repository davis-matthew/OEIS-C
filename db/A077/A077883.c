/*
 * A077883  Expansion of (1-x)^(-1)/(1-x^2+x^3).
 * https://oeis.org/A077883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 22, i16 n <= 42, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) - 2*a(n-3) + a(n-4)
   Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=1
   Computes a(n) for any non‑negative n.  The implementation
   computes the terms sequentially without heap allocation,
   so it works for all supported seq_t types (including
   arbitrary‑precision integer classes). */
seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)1;

    /* Roll the last four computed values: a0=a(n-4), a1=a(n-3),
       a2=a(n-2), a3=a(n-1). */
    seq_t a0 = (seq_t)1;          /* a(0)   */
    seq_t a1 = (seq_t)1;          /* a(1)   */
    seq_t a2 = (seq_t)2;          /* a(2)   */
    seq_t a3 = (seq_t)1;          /* a(3)   */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        ai = a3 + a2 - (seq_t)2 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
