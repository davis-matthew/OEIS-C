/*
 * A072690  a(n) = (n - A048760(n)) * (A048761(n) - n).
 * https://oeis.org/A072690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - floor(sqrt(n))^2) * (ceil(sqrt(n))^2 - n)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (A048760(n) = floor(sqrt(n))^2,  A048761(n) = ceil(sqrt(n))^2) */

#include <stdint.h>

/* integer square‑root of a 64‑bit unsigned integer (floor). */
static idx_t floor_sqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t lo = 0, hi = 1ULL << 32;  /* hi^2 > 2^64, enough for any 64‑bit n */
    if (hi > n) hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        uint64_t mid_low = mid;
        unsigned __int128 sq = (unsigned __int128)mid_low * mid_low;
        if (sq <= n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;   /* hi is floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    /* Compute floor sqrt and its square */
    idx_t fs = floor_sqrt_u64(n);
    seq_t fs_sq = (seq_t)((unsigned __int128)fs * fs);

    /* Determine ceil sqrt: if n is a perfect square, it equals fs */
    idx_t cs = fs;
    seq_t fs_sq_eq = fs_sq;
    if (fs_sq_eq != (seq_t)n) cs = fs + 1;
    seq_t cs_sq = (seq_t)((unsigned __int128)cs * cs);

    seq_t left  = (seq_t)n - fs_sq;
    seq_t right = cs_sq - (seq_t)n;

    return (seq_t)((unsigned __int128)left * right);
}
