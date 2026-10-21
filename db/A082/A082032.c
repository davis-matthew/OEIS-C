/*
 * A082032  Expansion of e.g.f.: exp(2*x)/(1-2*x).
 * https://oeis.org/A082032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*a(n‑1) + 2^n , a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A082032 using the recurrence relation.
   Works for any seq_t type that supports normal arithmetic operators. */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */
    seq_t a_prev = (seq_t)1;      /* a(0) */
    seq_t pow2   = (seq_t)1;      /* 2^0 */
    for (idx_t i = 1; i <= n; ++i) {
        pow2 <<= 1;                                       /* pow2 = 2^i */
        seq_t two_n = ((seq_t)2) * (seq_t)i;              /* 2*n  */
        a_prev = two_n * a_prev + pow2;                   /* a(i) */
    }
    return a_prev;                                         /* a(n) */
}
