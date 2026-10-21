/*
 * A234813  Number of distinct integers of the form i+(i+1)+(i+2)+...+j, for 1
 *          <= i <= j <= n.
 * https://oeis.org/A234813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct integers of the form i+(i+1)+...+j,
 *            with 1 <= i <= j <= n
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety for non‑positive input */

    /* Triangular number T_n, fits in 32 bits for n <= 10000 */
    uint32_t m = (uint32_t)n * (uint32_t)(n + 1) / 2;

    /* Bit array for 0 .. m (index 0 unused) */
    uint8_t *marked = (uint8_t*)calloc((size_t)m + 1, (size_t)1);
    if (!marked) return 0;  /* allocation failure */

    seq_t cnt = 0;

    for (idx_t d = 1; d <= n; ++d)
    {
        uint32_t offset = (uint32_t)d * (uint32_t)(d + 1) / 2;
        idx_t max_a = n - d;
        for (idx_t a = 0; a <= max_a; ++a)
        {
            uint32_t sum = offset + (uint32_t)d * (uint32_t)a;  /* 1 .. m */
            size_t idx = (size_t)sum;
            if (!marked[idx])
            {
                marked[idx] = 1;
                ++cnt;
            }
        }
    }

    free(marked);
    return cnt;
}
