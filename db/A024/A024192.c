/*
 * A024192  Integer part of (4th elementary symmetric function of
 *          3,4,...,n+5)/(3+4+...+n+5).
 * https://oeis.org/A024192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1/2880 * n (1 + n) (2 + n) (53848 + 26922 n + 5165
 *            n^2 + 450 n^3 + 15 n^4)/(8 + n))
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 92, u64 n <= 3900,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow on 64‑bit builds */
    __int128 t = n;
    __int128 poly = (__int128)53848
                    + (__int128)26922 * t
                    + (__int128)5165 * t * t
                    + (__int128)450 * t * t * t
                    + (__int128)15 * t * t * t * t;
    __int128 num = t * (t + 1) * (t + 2) * poly;
    __int128 denom = (__int128)2880 * (8 + t);
    seq_t result = (seq_t)(num / denom);   /* truncating division gives floor() */
    return result;
}
