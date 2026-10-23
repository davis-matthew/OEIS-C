/*
 * A014370  If n = binomial(b,2) + binomial(c,1), b > c >= 0 then a(n) =
 *          binomial(b+1,3) + binomial(c+1,2).
 * https://oeis.org/A014370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = binomial(b+1,3) + binomial(c+1,2) where n = binomial(b,2)
 *            + c, b > c >= 0 b is the largest integer with binomial(b,2) <= n
 *            c = n - binomial(b,2)
 * Range:     u8 n <= 62, u16 n <= 2665, u32 n <= 8094, u64 n <= 8094,
 *            u128 n <= 8094, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128 (floor of sqrt(x)) */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = (unsigned __int128)1 << 64;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid == 0) { low = 1; continue; }
        if (mid > x / mid) { high = mid - 1; }
        else { low = mid + 1; }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1; not used */

    /* compute b = floor((1 + sqrt(1 + 8*n)) / 2) */
    unsigned __int128 d = (unsigned __int128)n * 8 + 1;
    unsigned __int128 s = isqrt128(d);
    idx_t b = (idx_t)((s + 1) / 2);          /* b = floor((1+sqrt)/2) */

    /* triangular number T(b) = binomial(b,2) */
    seq_t t = (seq_t)b * (b - 1) / 2;       /* = binomial(b,2) */

    /* c = n - T(b) */
    idx_t c = (idx_t)((seq_t)n - t);

    /* a(n) = binomial(b+1,3) + binomial(c+1,2) */
    seq_t part1 = ((seq_t)(b + 1) * b * (b - 1)) / 6;   /* binom(b+1,3) */
    seq_t part2 = ((seq_t)(c + 1) * c) / 2;             /* binom(c+1,2) */

    return part1 + part2;
}
