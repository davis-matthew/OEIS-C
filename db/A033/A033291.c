/*
 * A033291  A Connell-like sequence: take the first multiple of 1, the next 2
 *          multiples of 2, the next 3 multiples of 3, etc.
 * https://oeis.org/A033291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabl
 * Formula:   a(n) = q(n)*n - q(n)*floor(q(n)*(q(n)+1)/6) where q(n) =
 *            ceil((sqrt(1+8*n)-1)/2). Indexing starts at 1
 * Range:     u8 n <= 43, u16 n <= 1699, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 80 terms through n = 4936 (2026-11-01)
 * Helpers:   isqrt_u128_to_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128_to_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but harmless for n>=1 */

    /* Compute q(n) = ceil((sqrt(1+8*n)-1)/2). */
    __uint128_t val = (__uint128_t)8 * n + 1;        /* 1 + 8n */
    idx_t s = isqrt_u128_to_idx(val);            /* floor sqrt */
    idx_t q = (s > 0) ? (s - 1) / 2 : 0;            /* floor((s-1)/2) */

    /* Ensure q satisfies q*(q+1)/2 >= n. */
    while (((__uint128_t)q * (q + 1)) / 2 < n) ++q;

    /* Compute floor(q*(q+1)/6). */
    idx_t part = ( (__uint128_t)q * (q + 1) ) / 6;

    /* Final value: q*n - q*part. */
    seq_t result = (seq_t)q * (seq_t)n - (seq_t)q * (seq_t)part;
    return result;
}
