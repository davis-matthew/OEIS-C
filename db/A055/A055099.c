/*
 * A055099  Expansion of g.f.: (1 + x)/(1 - 3*x - 2*x^2).
 * https://oeis.org/A055099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 4, a(n) = 3*a(n-1) + 2*a(n-2) for n >= 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A055099.  No dynamic allocation or
   static memoisation is used; the result is obtained by iteratively
   applying the recurrence, which is O(n) and works for any seq_t
   supporting integer arithmetic (built‑in or arbitrary‑precision) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    seq_t a0 = (seq_t)1;  /* a(n-2) */
    seq_t a1 = (seq_t)4;  /* a(n-1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        next = (seq_t)3 * a1 + (seq_t)2 * a0;
        a0   = a1;
        a1   = next;
    }

    return a1;
}
