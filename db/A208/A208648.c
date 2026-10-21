/*
 * A208648  Denominators of Pokrovskiy's lower bound on the ratio of e(G^n)
 *          the number of edges in the n-th power of a graph G, to E(G) the
 *          number of edges of G.
 * https://oeis.org/A208648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 1 if n ≠ 0 (mod 3) * a(n) = 2 * (n + 3) / 3 if n ≡ 0 (mod
 *            3) (denominator of Pokrovskiy's bound)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n % 3) != 0)          /* not divisible by 3 → denominator is 1 */
        return (seq_t)1;
    seq_t t = ((seq_t)n + (seq_t)3) / (seq_t)3;  /* (n+3)/3 is integer */
    return t * (seq_t)2;      /* 2*(n+3)/3 */
}
