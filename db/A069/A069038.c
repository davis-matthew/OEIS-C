/*
 * A069038  Expansion of g.f. x*(1+x)^4/(1-x)^6.
 * https://oeis.org/A069038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^4 + 10*n^2 + 3)/15
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 126, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The formula is valid for all n ≥ 0 and yields an integer.
       A 128‑bit intermediate is used to avoid overflow. */
    if (n == 0)
        return 0;

    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 n2 = N * N;          /* n^2   */
    unsigned __int128 n4 = n2 * n2;        /* n^4   */
    unsigned __int128 top = 2 * n4 + 10 * n2 + 3;
    unsigned __int128 res = N * top / 15;  /* exact division */

    return (seq_t)res;
}
