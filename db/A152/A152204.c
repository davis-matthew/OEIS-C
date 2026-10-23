/*
 * A152204  Triangle read by rows: T(n,k) = 2*n-4*k+5 (n >= 0, 1 <= k <=
 *          1+floor(n/2)).
 * https://oeis.org/A152204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = T(r,k) where * r = ⌈√(4(n+1))⌉ – 2, * k = n – ⌊(r+1)²/4⌋
 *            + 1, * T(r,k) = 2 r – 4 k + 5
 * Range:     u8 n <= 4159, u16 n <= 8906, u32 n <= 8906, u64 n <= 8906,
 *            u128 n <= 8906, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Compute r = ceil(sqrt(4*(n+1))) - 2 */
    __int128 p = (__int128)n + 1;          /* (n+1) as 128‑bit */
    __int128 q = (__int128)4 * p;          /* 4*(n+1) */
    long double f = sqrt((long double)q);
    unsigned long long t = (unsigned long long)f;
    /* adjust to obtain ceil(sqrt(q)) */
    while ((__int128)t * t < q) t++;
    while (t > 0 && (__int128)(t - 1) * (t - 1) >= q) t--;
    idx_t r = t - 2;

    /* Compute S(r-1) = ⌊(r+1)²/4⌋ */
    unsigned long long rp1 = r + 1;        /* r-1+2 = r+1 */
    __int128 prod = (__int128)rp1 * rp1;
    idx_t Sprev = (idx_t)(prod / 4);

    /* Column position k */
    idx_t k = n - Sprev + 1;

    /* Triangle value T(r,k) = 2 r - 4 k + 5 */
    seq_t val = (seq_t)((seq_t)2 * (seq_t)r - (seq_t)4 * (seq_t)k + (seq_t)5);
    return val;
}
