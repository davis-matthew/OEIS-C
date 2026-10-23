/*
 * A008861  a(n) = Sum_{k=0..8} binomial(n,k).
 * https://oeis.org/A008861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..8} binomial(n,k)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 62, u64 n <= 966,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial for small k (k <= 8) */
static inline seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)          /* use symmetry */
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* use 128‑bit intermediate to avoid overflow for 64‑bit seq_t */
        unsigned __int128 prod = (unsigned __int128)res * (n - k + i);
        res = (seq_t)(prod / i);
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= 8; ++k)
        sum += binom_small(n, k);
    return sum;
}
