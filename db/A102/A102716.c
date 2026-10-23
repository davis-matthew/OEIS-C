/*
 * A102716  Triangle read by rows: T(n,k) = sigma(binomial(n,k)) (0 <= k <=
 *          n), where sigma(m) is the sum of the positive divisors of m.
 * https://oeis.org/A102716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sigma(binomial(row,col)) * where the triangle
 *            T(m,k)=sigma(binomial(m,k)) is flattened row by row
 * Range:     u8 n <= 48, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   exp_factorial, pow_uint32, sigma_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/exp_factorial.h"
#include "../utils/pow_uint32.h"
#include "../utils/sigma_binom.h"

/* main sequence function */
seq_t a(idx_t n)
{
    /* find row r and column k such that
       cumulative = sum_{i=0}^{r-1} (i+1) <= n < sum_{i=0}^{r} (i+1) */
    idx_t cumulative = 0;
    idx_t r = 0;
    for (;; ++r)
    {
        idx_t next = cumulative + (r + 1);
        if (n < next)
            break;
        cumulative = next;
    }
    idx_t k = n - cumulative;
    return sigma_binom(r, k);
}
