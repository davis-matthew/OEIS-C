/*
 * A031449  Numbers whose base-2 representation has two fewer 0's than 1's.
 * https://oeis.org/A031449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A031449
   (numbers whose binary representation has two more 1's than 0's). */
#include <stdint.h>

static idx_t binom_idx(idx_t n, idx_t k)
{
    if (k > n/2) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        __uint128_t tmp = (__uint128_t)res * (n - k + i);
        res = (idx_t)(tmp / i);
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 1 */

    idx_t k = 1;
    idx_t cumulative = 0;
    while (1)
    {
        idx_t count = binom_idx(2*k-1, k-1);
        if (cumulative + count >= n)
            break;
        cumulative += count;
        ++k;
    }

    idx_t rank = n - cumulative - 1;       /* 0‑based rank in this group */
    idx_t zeros_total = k - 1;
    seq_t result = 1;                      /* leading '1' */
    idx_t zeros_selected = 0;

    for (idx_t pos = 1; pos < 2*k; ++pos)
    {
        idx_t remaining = (2*k-1) - pos;   /* bits after this position */
        idx_t zeros_needed = zeros_total - zeros_selected;
        idx_t count0 = 0;
        if (zeros_needed > 0)
            count0 = binom_idx(remaining, zeros_needed-1);

        if (rank < count0)
        {
            result <<= 1;                 /* put '0' bit */
            ++zeros_selected;
        }
        else
        {
            result = (result << 1) | 1;    /* put '1' bit */
            rank -= count0;
        }
    }
    return result;
}
