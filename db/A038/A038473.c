/*
 * A038473  Sums of 5 distinct powers of 4.
 * https://oeis.org/A038473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 56, u32 n <= 4368, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = sum of 5 distinct powers of 4.
   The n‑th term is obtained by mapping (n-1) to the
   lexicographic 5‑combination of indices using the
   combinatorial number system and adding 4^exponent
   for each chosen exponent.
*/
#include "../utils/pow4.h"

static inline idx_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t rank = n - 1;
    seq_t sum = 0;
    const idx_t K = 5;
    idx_t remaining = K;
    while (remaining > 0)
    {
        idx_t i = remaining;
        while (binom(i, remaining) <= rank) ++i;
        idx_t exp = i - 1;
        sum += pow4(exp);
        rank -= binom(i - 1, remaining);
        --remaining;
    }
    return sum;
}
