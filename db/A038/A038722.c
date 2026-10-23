/*
 * A038722  Take the sequence of natural numbers (A000027) and reverse
 *          successive subsequences of lengths 1,2,3,4,... .
 * https://oeis.org/A038722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = ceil((sqrt(8*n + 1) - 1)/2)^2 - n + 1 Equivalent integer
 *            version: t = floor_sqrt(8*n + 1); if t*t == 8*n + 1 then k =
 *            (t-1)/2 else k = (t-1)/2 + 1 a(n) = k*k - n + 1
 * Range:     u8 n <= 7, u16 n <= 1669, u32 n <= 9143, u64 n <= 9143,
 *            u128 n <= 9143, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;                     /* cast index to seq_t */
    seq_t v  = nn * (seq_t)8 + 1;            /* 8*n + 1 */
    seq_t s  = floor_sqrt(v);                /* floor(sqrt(8*n+1)) */
    seq_t k  = (s - 1) / 2;                  /* floor((s-1)/2) */
    /* If 8*n+1 is not a perfect square, ceil is floor + 1 */
    if (s * s != v) {
        k += 1;
    }
    seq_t a_val = k * k - nn + 1;           /* k^2 - n + 1 */
    return a_val;
}
