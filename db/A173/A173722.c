/*
 * A173722  Partial sums of round(n^2/8).
 * https://oeis.org/A173722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2)*(2*n^2 - n + 6)/48)
 * Range:     u8 n <= 17, u16 n <= 115, u32 n <= 4688, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t n64 = n;                           /* cast to 64‑bit for intermediates   */
    uint64_t term1 = 2ULL * n64 * n64;          /* 2 * n^2                             */
    uint64_t term2 = term1 - n64 + 6ULL;        /* 2*n^2 - n + 6                       */
    uint64_t numerator = (n64 + 2ULL) * term2;  /* (n+2)*(2*n^2-n+6)                   */
    uint64_t result64 = numerator / 48ULL;      /* floor division by 48                */
    return (seq_t)result64;                     /* cast back to sequence type          */
}
