/*
 * A189368  a(n) = n + [n*r/s] + [n*t/s]; r=2, s=sqrt(2), t=sqrt(3).
 * https://oeis.org/A189368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*√2⌋ + ⌊n*√(3/2)⌋
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the floor of the square root of a 128‑bit unsigned integer.
   The result fits in seq_t (which is at least 64‑bit). */
static inline seq_t sqrt_u128(unsigned __int128 val) {
    unsigned __int128 lo = 0;
    unsigned __int128 hi = ((unsigned __int128)1 << 64) - 1;
    unsigned __int128 best = 0;
    while (lo <= hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= val) {
            best = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;   /* avoid underflow */
            hi = mid - 1;
        }
    }
    return (seq_t)best;
}

/* Sequence A189368: a(n) = n + ⌊n*√2⌋ + ⌊n*√(3/2)⌋ */
seq_t a(idx_t n) {
    /* Convert index to 128‑bit for intermediate arithmetic */
    unsigned __int128 u_n = (unsigned __int128)n;

    /* ⌊n*√2⌋  = ⌊√(2*n²)⌋ */
    unsigned __int128 val1 = u_n * u_n * 2ULL;
    seq_t term1 = sqrt_u128(val1);

    /* ⌊n*√(3/2)⌋ = ⌊√(3*n²/2)⌋  (use integer division first) */
    unsigned __int128 val2 = u_n * u_n * 3ULL;
    val2 /= 2ULL;
    seq_t term2 = sqrt_u128(val2);

    return (seq_t)n + term1 + term2;
}
