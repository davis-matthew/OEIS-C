/*
 * A089091  a(n) is the smallest composite number coprime to n and n+1.
 * https://oeis.org/A089091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (smallest integer >1 coprime to n and n+1)^2
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd_uint64_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_uint64_ull.h"

/* Return the nth term of OEIS A089091 */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;          /* cast index to a native type */
    uint64_t d = 2;                    /* candidate divisor strictly > 1 */

    for (;;) {
        /* Check coprimality with n and n+1 */
        if (gcd_uint64_ull(N, d) == 1 && gcd_uint64_ull(N + 1, d) == 1)
            break;
        ++d;
    }

    /* Compute d^2 in seq_t; cast to seq_t early to avoid overflow of uint64_t */
    return (seq_t)d * (seq_t)d;
}
