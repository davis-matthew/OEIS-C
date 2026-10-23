/*
 * A162877  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 38*a(n-1) + 38*a(n-2) - 741*a(n-3) for n ≥ 4; Initial
 *            values: a(0)=1, a(1)=40, a(2)=1560, a(3)=60060
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)40;
    if (n == 2) return (seq_t)1560;
    if (n == 3) return (seq_t)60060;

    /* use a sliding window of the last three terms */
    seq_t a1 = (seq_t)40;          /* a(n-3) for current i */
    seq_t a2 = (seq_t)1560;        /* a(n-2) for current i */
    seq_t a3 = (seq_t)60060;       /* a(n-1) for current i */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = (seq_t)38 * a3 + (seq_t)38 * a2 - (seq_t)741 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
