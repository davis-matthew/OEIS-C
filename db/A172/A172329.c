/*
 * A172329  a(n) = floor(n*(sqrt(11) + sqrt(2))).
 * https://oeis.org/A172329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = floor(n*(sqrt(11) + sqrt(2)))
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute integer floor of sqrt for a 64‑bit value.
   Works because all arguments in this sequence are <= 2^32. */
static inline seq_t isqrt64(uint64_t x)
{
    uint64_t lo = 0, hi = (x >> 1) + 1, mid, sq;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        sq = mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return (seq_t)(lo - 1);
}

seq_t a(idx_t n)
{
    /* Special case to avoid unnecessary work */
    if (n == 0) return (seq_t)0;

    /* Work in 64‑bit to avoid overflow for n <= 10000 */
    uint64_t n64 = (uint64_t)n;
    uint64_t w64 = n64 * n64;                /* w = n^2 */

    uint64_t t1 = 11 * w64;                 /* 11*n^2 */
    uint64_t t2 = 2 * w64;                  /*  2*n^2 */

    seq_t u = isqrt64(t1);                  /* floor(n*sqrt(11)) */
    seq_t v = isqrt64(t2);                  /* floor(n*sqrt(2)) */

    seq_t s = u + v;                        /* sum of integer parts */
    seq_t L = s + (seq_t)1;                 /* candidate for floor(n*alpha)+1 */

    /* A = L^2 - 13*w, signed to detect negative case */
    int64_t A = (int64_t)((uint64_t)L * (uint64_t)L) - (int64_t)(13 * w64);

    uint64_t carry = 0;
    if (A < 0) {
        carry = 1;                          /* fractional parts definitely add to >=1 */
    } else {
        /* Compare A^2 <= 88*w^2 using 128‑bit arithmetic */
        unsigned __int128 left  = (unsigned __int128)A * (unsigned __int128)A;
        unsigned __int128 right = (unsigned __int128)88 * (unsigned __int128)w64 * (unsigned __int128)w64;
        if (left <= right) carry = 1;
    }

    return s + (seq_t)carry;                /* final value floor(n*alpha) */
}
