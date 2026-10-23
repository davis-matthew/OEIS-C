/*
 * A168010  a(n) = Sum of all numbers of divisors of all numbers k such that
 *          n^2 <= k < (n+1)^2.
 * https://oeis.org/A168010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Σ_{i=1..n} (⌊(n^2+2n)/i⌋ - ⌊(n^2-1)/i⌋) - 1
 * Range:     u8 n <= 15, u16 n <= 506, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A168010 */
seq_t a(idx_t n)
{
    /* Convert n to seq_t for all arithmetic */
    seq_t nn = (seq_t)n;
    /* Pre‑compute constants */
    seq_t N1 = nn * nn + (seq_t)2 * nn;      /* n^2 + 2n   */
    seq_t N0 = nn * nn - (seq_t)1;           /* n^2 - 1    */

    seq_t sum = (seq_t)0;
    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        seq_t ii = (seq_t)i;
        seq_t part = (N1 / ii) - (N0 / ii); /* floor division */
        sum += (seq_t)2 * part;
    }
    sum -= (seq_t)1;
    return sum;
}
