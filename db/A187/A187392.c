/*
 * A187392  Floor(s*n), where s=1+sqrt(8)-sqrt(7); complement of A189391.
 * https://oeis.org/A187392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1+sqrt(8)-sqrt(7))*n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root for 64‑bit values */
static inline uint64_t isqrt_u64(uint64_t v)
{
    uint64_t lo = 0, hi = (1ULL << 32);
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= v)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* n^2 as 64‑bit value (safe for n <= 10 000) */
    uint64_t n64   = (uint64_t)n;
    uint64_t n_sq64 = n64 * n64;

    /* floor(sqrt(8*n^2)) and floor(sqrt(7*n^2)) */
    uint64_t a_val = isqrt_u64(8ULL * n_sq64);
    uint64_t b_val = isqrt_u64(7ULL * n_sq64);

    /* differences of the integer parts */
    __int128 diff_ab = (__int128)a_val - (__int128)b_val;

    /* temporary values for the inequality */
    __int128 n_sq  = (__int128)n_sq64;
    __int128 tmp1  = (__int128)15 * n_sq;      /* 15*n^2   */
    __int128 tmp2  = diff_ab * diff_ab;        /* (diff)^2 */
    __int128 u     = tmp1 - tmp2;              /* u = 15*n^2 – (diff)^2 */

    __int128 u_sq = u * u;                     /* u^2      */
    __int128 n4   = n_sq * n_sq;               /* n^4      */
    __int128 R    = (__int128)224 * n4;        /* 224*n^4  */

    bool f1_lt_f2 = u_sq < R;

    /* floor(n*sqrt8 - n*sqrt7) = diff_ab or diff_ab-1 */
    __int128 diff_floor = diff_ab;
    if (f1_lt_f2)
        diff_floor -= 1;

    /* a(n) = n + floor(n*sqrt8 - n*sqrt7) */
    seq_t n_seq = (seq_t)n;
    return n_seq + (seq_t)diff_floor;
}
