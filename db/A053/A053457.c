/*
 * A053457  Open disk numbers (version 2): a(n) is the number of points (i,j),
 *          i,j, integers, contained in an open disk of diameter n, centered
 *          at (0,1/2).
 * https://oeis.org/A053457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   int_sqrt_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A053457 – Open disk numbers (version 2): 
   a(n) is the number of integer lattice points (i,j) contained
   in the open disk of diameter n, centered at (0, 1/2).
   The formula used is
     a(n) = Σ_{i = -⌊(n-1)/2⌋}^{⌊(n-1)/2⌋}
            (number of integers j such that
             i² + (j-½)² < (n/2)²),
   which is equivalent to
     a(n) = Σ_{i = 0}^{⌊(n-1)/2⌋}
            ( (i==0?1:2) *
              2 * ⌊ ( √(n² - 4i² - 1) + 1 ) / 2 ⌋ )
*/

#include <stdint.h>
#include "../utils/int_sqrt_ull.h"

seq_t a(idx_t n)
{
    if (n <= 1)               /* for n = 0 or 1 the disk contains no interior lattice points */
        return (seq_t)0;

    const unsigned long long N = (unsigned long long)n;
    const unsigned long long NN = N * N;   /* n², fits in 64‑bit for all n used by the tests */

    unsigned long long total = 0;
    for (unsigned long long i = 0;; ++i) {
        unsigned long long four_i2 = 4ULL * i * i;
        if (four_i2 >= NN)          /* i² ≥ (n/2)², no further points possible */
            break;

        unsigned long long val = NN - four_i2;          /* = (n/2)² - i²  scaled by 4 */
        /* We need the number of odd integers m with m² < val.
           Let M = floor( √(val-1) ); then the count is 2 * floor((M+1)/2). */
        unsigned long long M = int_sqrt_ull(val - 1);
        unsigned long long cnt = 2ULL * ((M + 1ULL) / 2ULL);   /* number of j for this i */

        if (i == 0)
            total += cnt;           /* only one vertical line when i = 0 */
        else
            total += 2ULL * cnt;    /* count both i and -i */
    }

    return (seq_t)total;
}
