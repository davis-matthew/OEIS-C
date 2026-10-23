/*
 * A033275  Number of diagonal dissections of an n-gon into 3 regions.
 * https://oeis.org/A033275
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1, 2) * binomial(n-3, 2) / 3
 * Range:     u8 n <= 9, u16 n <= 31, u32 n <= 477, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;                     /* values below the offset are irrelevant */

    /* Compute ((n+1) * n * (n-3) * (n-4)) / 12
       using a wider type to avoid intermediate overflow. */
    __int128 prod = (__int128)(n + 1);
    prod *= n;
    prod *= (n - 3);
    prod *= (n - 4);
    prod /= 12;                              /* 12 = 2*2*3, the same denominator as the formula */

    return (seq_t)prod;
}
