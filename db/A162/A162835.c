/*
 * A162835  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29*a(n-1) + 29*a(n-2) - 435*a(n-3) for n ≥ 4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=1, a(1)=31, a(2)=930, a(3)=27435 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)31;
    if (n == 2) return (seq_t)930;
    if (n == 3) return (seq_t)27435;

    seq_t prev1 = (seq_t)27435; /* a(n-1) at start */
    seq_t prev2 = (seq_t)930;   /* a(n-2) */
    seq_t prev3 = (seq_t)31;    /* a(n-3) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i)
    {
        next = ((seq_t)29 * prev1) + ((seq_t)29 * prev2) - ((seq_t)435 * prev3);
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
