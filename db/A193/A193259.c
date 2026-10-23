/*
 * A193259  G.f.: x = Sum_{n>=1} x^n * ((1+x)^n - x^n) / (1+x)^a(n).
 * https://oeis.org/A193259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(log_2(n)) + v2(n!) where v2(n!) = sum_{k>=1}
 *            floor(n/2^k)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, if needed */

static seq_t floor_log2_idx(idx_t x)
{
    seq_t r = 0;
    while (x > 1)
    {
        x >>= 1;
        r++;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for n < 1, return 0 */
        return 0;

    seq_t result = (seq_t)n;
    result += floor_log2_idx(n);

    seq_t v2 = 0;
    idx_t m = n;
    while (m > 0)
    {
        m >>= 1;
        v2 += (seq_t)m;
    }

    result += v2;
    return result;
}
