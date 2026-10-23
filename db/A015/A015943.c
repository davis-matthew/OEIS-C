/*
 * A015943  a(n) = (2^(2*n)+n) mod (2*n).
 * https://oeis.org/A015943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(2*n)+n) mod (2*n)
 * Range:     u8 n <= 159, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 2^(2*n) mod (2*n) as (4^n) mod (2*n) */
    seq_t mod = (seq_t)(2 * n);
    uint64_t r = 1 % (uint64_t)mod;          /* current 4^k mod mod */
    for (idx_t i = 0; i < n; ++i)
        r = (r * 4) % (uint64_t)mod;        /* r stays <= mod-1 <= 2*n-1 */

    seq_t result = ((seq_t)r + (seq_t)n) % mod;
    return result;
}
