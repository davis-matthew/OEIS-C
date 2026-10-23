/*
 * A165457  a(n) = (2*n+1)!*(2*n+3)/3.
 * https://oeis.org/A165457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)! * (2*n+3) / 3 Recurrence: a(0) = 1 a(n) =
 *            2*n*(2*n+3)*a(n-1) for n >= 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS sequence A165457.
   Uses the multiplicative recurrence, computes on-the-fly.
   Works for any seq_t and idx_t provided by the build. */
seq_t a(idx_t n)
{
    /* start with a(0) = 1 */
    seq_t value = 1;
    /* compute a(1) .. a(n) using recurrence:
       a(i) = 2*i*(2*i+3)*a(i-1)
    */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* factor = 2*i*(2*i+3) */
        seq_t factor = (seq_t)2 * (seq_t)i;
        factor *= ((seq_t)2 * (seq_t)i + (seq_t)3);
        value *= factor;
    }
    return value;
}
