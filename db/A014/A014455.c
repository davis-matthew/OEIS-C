/*
 * A014455  Theta series of quadratic form with Gram matrix [ 1, 0, 0; 0, 1,
 *          0; 0, 0, 2 ]. Number of integer solutions to x^2 + y^2 + 2*z^2 =
 *          n.
 * https://oeis.org/A014455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = # {(x,y,z)∈ℤ³ : x² + y² + 2 z² = n}
 * Range:     u8 n <= 537, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache of r₂ values up to the largest n seen so far */
    static idx_t pre_limit = 0;
    static seq_t *r2_arr = NULL;

    /* If this is the first call or we need more data, rebuild the cache */
    if (pre_limit == 0 || n > pre_limit)
    {
        size_t new_limit = (size_t)n;
        size_t sz = new_limit + 1;

        /* Count divisors ≡1 (mod 4) and ≡3 (mod 4) for all m ≤ new_limit */
        uint32_t *d1 = (uint32_t*)calloc(sz, sizeof(uint32_t));
        uint32_t *d3 = (uint32_t*)calloc(sz, sizeof(uint32_t));
        for (size_t d = 1; d <= new_limit; ++d)
        {
            uint8_t mod = d & 3u;          /* d % 4 */
            if (mod == 1u)
            {
                for (size_t m = d; m <= new_limit; m += d) d1[m] += 1;
            }
            else if (mod == 3u)
            {
                for (size_t m = d; m <= new_limit; m += d) d3[m] += 1;
            }
        }

        /* Build r₂ array: r₂(n) = 4*(d1(n) − d3(n)) for n>0, r₂(0)=1 */
        seq_t *new_r2 = (seq_t*)malloc(sz * sizeof(seq_t));
        new_r2[0] = 1;
        for (size_t i = 1; i <= new_limit; ++i)
        {
            new_r2[i] = (seq_t)4 * ((seq_t)d1[i] - (seq_t)d3[i]);
        }

        free(r2_arr);
        free(d1);
        free(d3);

        r2_arr = new_r2;
        pre_limit = new_limit;
    }

    /* Sum over all z≥0, counting both signs of z when z≠0 */
    seq_t ans = 0;
    for (idx_t z = 0; ; ++z)
    {
        idx_t two_z_sq = 2 * z * z;
        if (two_z_sq > n) break;
        idx_t r = n - two_z_sq;
        seq_t val = r2_arr[(size_t)r];
        if (z == 0)
            ans += val;           /* single sign for z=0 */
        else
            ans += val + val;     /* two signs for z≠0 */
    }

    return ans;
}
