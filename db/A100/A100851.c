/*
 * A100851  Triangle read by rows: T(n,k) = 2^n * 3^k, 0 <= k <= n, n >= 0.
 * https://oeis.org/A100851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2^m * 3^k where m = floor((sqrt(8n+1)-1)/2) * and k = n -
 *            m(m+1)/2. This is the triangle T(m,k)=2^m*3^k * read by rows
 *            (0-based indexing)
 * Range:     u8 n <= 12, u16 n <= 33, u32 n <= 102, u64 n <= 349,
 *            u128 n <= 1324, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"

/* Compute 2^exp in seq_t, using exponentiation by squaring. */
static inline seq_t pow2(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* External linkage as required: public sequence function. */
seq_t a(idx_t n)
{
    /* Find the row m such that n lies in that row. */
    idx_t m = 0;
    __uint128_t count = 0;                 /* terms before current row   */
    while ( ( __uint128_t )( count + m + 1 ) <= n )
    {
        count += m + 1;
        ++m;
    }
    idx_t k = n - (idx_t)count;            /* column in the found row     */
    return pow2(m) * pow3(k);
}
