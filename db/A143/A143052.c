/*
 * A143052  Inverse permutation to A143051.
 * https://oeis.org/A143052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n (for n < 2); if n = m^2 with m >= 2, a(n) = n -
 *            2*(m-1); otherwise a(n) = n + 1
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Helper: integer square root for uint64_t values. */
#include <math.h>

/* Compute floor(sqrt(n)) for 0 <= n < 2^64. */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t r = (idx_t) sqrt((double) n);
    /* Adjust downwards if needed. */
    while ((unsigned __int128) r * r > n) --r;
    /* Adjust upwards if needed. */
    while ((unsigned __int128) (r + 1) * (r + 1) <= n) ++r;
    return r;
}

seq_t a(idx_t n) {
    if (n < 2) {
        return (seq_t) n;  /* a(0)=0, a(1)=1 */
    }
    idx_t r = isqrt_u64(n);
    if ((unsigned __int128) r * r == n) {        /* n is a perfect square */
        return (seq_t)(n - 2 * (r - 1));
    } else {
        return (seq_t)(n + 1);
    }
}
