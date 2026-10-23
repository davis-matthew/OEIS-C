/*
 * A118132  Triangle read by rows n>=0: the largest prime <= m^n+2 in columns
 *          m=3..n+3.
 * https://oeis.org/A118132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = largest prime ≤ (m^r + 2), where r = floor( (-1 + sqrt(8n
 *            + 1)) / 2 ) (row number) k = n - r(r + 1)/2 (index within row) m
 *            = 3 + k (column base)
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 24, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/is_prime_big.h"
#include "../utils/big_pow.h"

static seq_t prev_prime(seq_t x)
{
    seq_t cand = x;
    for (;;)
    {
        if (is_prime_big(cand))
            return cand;
        cand--;
    }
}

seq_t a(idx_t n)
{
    /* Determine the row r of index n */
    idx_t r = (idx_t)((sqrt(8.0 * n + 1.0) - 1.0) / 2.0);

    /* Compute the start index of this row */
    idx_t start = r * (r + 1) / 2;

    /* Position k within the row */
    idx_t k = n - start;

    /* Column base m */
    idx_t m = 3 + k;

    /* Compute m^r + 2 */
    seq_t val = big_pow((seq_t)m, (seq_t)r) + (seq_t)2;

    /* Find the largest prime ≤ val */
    return prev_prime(val);
}
