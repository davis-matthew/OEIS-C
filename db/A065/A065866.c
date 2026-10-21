/*
 * A065866  a(n) = n! * Catalan(n+1).
 * https://oeis.org/A065866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Catalan(n+1) = 2*(2n+1)!/(n+2)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 1;
                for i ≥ 1:  a(i) = a(i‑1) * 2*i*(2*i+1)/(i+2)   */
seq_t a(idx_t n)
{
    seq_t result = 1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t num = (seq_t)2 * i;                 /* 2*i */
        num *= (seq_t)2 * i + 1;                  /* 2*i*(2*i+1) */
        result = (result * num) / (i + 2);        /* divide exactly */
    }
    return result;
}
