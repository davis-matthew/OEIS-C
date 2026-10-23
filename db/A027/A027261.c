/*
 * A027261  a(n) = Sum_{k=0..2n} (k+1) * A025177(n, k).
 * https://oeis.org/A027261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Sum_{k=0..2n} (k+1) * A025177(n, k).
   This satisfies the recurrence
       a(0) = 1, a(1) = 4, a(2) = 18,
       a(n) = 6*a(n-1) - 9*a(n-2)   for n ≥ 3.
   The recurrence follows from the generating function
   (1 - 9*x^2 + 18*x^3)/(1 - 3*x)^2. */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)4;
    if (n == 2u) return (seq_t)18;

    seq_t a_nm2 = (seq_t)4;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)18;  /* a(n-1)  */
    seq_t a_n   = a_nm1;      /* placeholder for a(n) */

    for (idx_t i = 3u; i <= n; ++i) {
        a_n   = (seq_t)6 * a_nm1 - (seq_t)9 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
