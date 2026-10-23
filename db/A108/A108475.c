/*
 * A108475  Expansion of (1-3*x) / (1-5*x-5*x^2+x^3).
 * https://oeis.org/A108475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A108475
   Recurrence: a(0) = 1, a(1) = 2, a(2) = 15
                a(n) = 5*a(n-1) + 5*a(n-2) - a(n-3)   (n ≥ 3)
   This implementation evaluates the recurrence iteratively; no
   dynamic arrays or memoisation are required.  It works for any
   integer type supplied as seq_t and idx_t, including arbitrary‑precision
   integers on the C++ build.  All intermediate values are kept in seq_t
   and only +, -, * and a cast of the small constant 5 to seq_t are used. */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)2;
    if (n == 2U) return (seq_t)15;

    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)2;   /* a(n-2)   */
    seq_t a2 = (seq_t)15;  /* a(n-1)   */
    for (idx_t i = 3U; i <= n; ++i) {
        seq_t next = (seq_t)5 * a2 + (seq_t)5 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
