/*
 * A016161  Expansion of g.f. 1/((1-5*x)*(1-7*x)).
 * https://oeis.org/A016161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7^(n+1) - 5^(n+1)) / 2 Recurrence: a(0) = 1; a(n) = 7 *
 *            a(n-1) + 5^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;

    seq_t a_val = (seq_t)1;        /* a(0) */
    seq_t pow5  = (seq_t)5;        /* 5^1 */
    for (idx_t i = 1U; i <= n; ++i) {
        a_val = ((seq_t)7) * a_val + pow5;   /* a(i) = 7*a(i-1) + 5^i */
        pow5 *= (seq_t)5;                    /* next power of 5 */
    }
    return a_val;
}
