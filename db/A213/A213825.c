/*
 * A213825  Rectangular array: (row n) = b**c, where b(h) = 3*h-1, c(h) =
 *          3*n-5+3*h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (3(r+c)-2)*c(3c+1)/2 - 3c^3 - 3c^2 where r and c are
 *            derived from the anti‑diagonal index: let s be the unique
 *            integer ≥2 with (s-1)*s/2 ≥ n; let r = n - (s-2)(s-1)/2; c = s -
 *            r
 * Range:     u8 n <= 15, u16 n <= 630, u32 n <= 3240, u64 n <= 3240,
 *            u128 n <= 3240, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 u128;
#else
typedef unsigned long long u128;   /* fallback: 64‑bit, assumed large enough for intermediate */
#endif

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Find the diagonal index s such that (s-1)*s/2 >= n */
    idx_t s = 2;
    while ((u128)(s - 1) * (u128)s / 2 < n)
        ++s;

    /* Position on that diagonal: r, c */
    u128 pref = (u128)(s - 2) * (u128)(s - 1) / 2; /* elements before diagonal s */
    idx_t r = n - (idx_t)pref;
    idx_t c = s - r;

    /* Compute T(r,c) by the closed formula */
    seq_t rr = (seq_t)r;
    seq_t cc = (seq_t)c;
    seq_t t  = (seq_t)(3 * (rr + cc) - 2);

    u128 part1 = (u128)t * (u128)cc * (u128)(3 * cc + 1) / 2;
    u128 part2 = (u128)3 * (u128)cc * (u128)cc * (u128)cc
               + (u128)3 * (u128)cc * (u128)cc;

    return (seq_t)(part1 - part2);
}
