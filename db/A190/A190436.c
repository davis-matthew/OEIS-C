/*
 * A190436  a(n) = [(b*n+c)*r] - b*[n*r] - [c*r], where (r,b,c)=(golden
 *          ratio,3,2) and []=floor.
 * https://oeis.org/A190436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [(b*n+c)*r] - b*[n*r] - [c*r], with r = (1+sqrt(5))/2, b
 *            = 3, c = 2
 * Range:     u8 n <= 37, u16 n <= 9845, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 131 terms through n = 9846 (2026-11-01)
 * Helpers:   isqrt_u128_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128_to_u64.h"

seq_t a(idx_t n)
{
    /* Compute floor((3*n+2)*phi) */
    idx_t k1 = 3*n + 2;
    unsigned __int128 val1 = (unsigned __int128)k1 * k1 * 5;  /* 5*k1^2 */
    uint64_t s1 = isqrt_u128_to_u64(val1);                  /* floor(k1*sqrt5) */
    seq_t term1 = ((seq_t)s1 + (seq_t)k1) / 2;               /* floor(k1*phi) */

    /* Compute floor(n*phi) */
    unsigned __int128 val2 = (unsigned __int128)n * n * 5;  /* 5*n^2 */
    uint64_t s2 = isqrt_u128_to_u64(val2);                 /* floor(n*sqrt5) */
    seq_t term2 = ((seq_t)s2 + (seq_t)n) / 2;               /* floor(n*phi) */

    seq_t result = term1 - 3*term2 - 3;
    return result;
}
