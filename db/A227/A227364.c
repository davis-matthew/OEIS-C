/*
 * A227364  a(n) = 1 + 2*3 + 4*5*6 + 7*8*9*10 + ... + ...*n (see Example
 *          lines).
 * https://oeis.org/A227364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{∞} (Π_{i=1+(k-1)k/2}^{min(1+(k-1)k/2 + k-1, n)}
 *            i)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 65, u128 n <= 170,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

int64_t seq_from_str(const char *);

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t prod = 1;
    idx_t k = 1;
    idx_t start = 1;
    idx_t end = start + k - 1;

    for (idx_t i = 1; i <= n; ++i)
    {
        prod = prod * (seq_t)i;
        if (i == end)
        {
            sum += prod;
            ++k;
            start = end + 1;
            end   = start + k - 1;
            prod  = 1;
        }
    }
    if (prod != 1)  /* last group incomplete */
        sum += prod;

    return sum;
}
