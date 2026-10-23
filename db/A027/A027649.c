/*
 * A027649  a(n) = 2*(3^n) - 2^n.
 * https://oeis.org/A027649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*(3^n) - 2^n
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(n) = 3*a(n-1) + 2^(n-1) */

seq_t a(idx_t n)
{
    seq_t a_val   = 1;          /* a(0) */
    seq_t pow2_val = 1;         /* 2^(n-1) for the current iteration */

    for (idx_t i = 1; i <= n; ++i)
    {
        a_val   = a_val * ((seq_t)3) + pow2_val;   /* a(i) = 3*a(i-1) + 2^(i-1) */
        pow2_val = pow2_val * ((seq_t)2);          /* update 2^(i) for next round */
    }

    return a_val;
}
