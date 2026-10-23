/*
 * A147650  First trisection of A061040.
 * https://oeis.org/A147650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 if n ≡ 1 or 8 (mod 9) a(n) = 3*n^2 if n ≡ 1 or 2 (mod
 *            3) a(n) = 9*n^2 if n ≡ 0 (mod 3) (Derived from the denominator
 *            of (n-1)*(n+1)/(9*n^2))
 * Range:     u8 n <= 5, u16 n <= 86, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned; n ≥ 1 per specification */
    idx_t mod9 = n % 9;
    seq_t n_sq = (seq_t)n * (seq_t)n;

    if (mod9 == 1 || mod9 == 8)
        return n_sq;

    if ((n % 3) == 0)
        return (seq_t)9 * n_sq;
    else
        return (seq_t)3 * n_sq;
}
