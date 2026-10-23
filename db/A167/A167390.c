/*
 * A167390  Partial sums of A137442.
 * https://oeis.org/A167390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n)*(1 + T(n)*(3 + 2*T(n)))/6 + U( floor(n/2) ) where
 *            T(n) = ceil(n/2) U(m) = R*(R+1)/2 - S*(S+1)*(2*S+1)/6, R = m +
 *            round( sqrt(m) ), S = floor( sqrt(R) )
 * Range:     u8 n <= 15, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)).  */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;   /* sqrt(2^64‑1) < 2^32  */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid != 0 && mid > n / mid) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return hi;
}

/* Compute U(m) defined in the OEIS entry.  */
static seq_t compute_U(idx_t m)
{
    /* round(sqrt(m)) */
    idx_t fs = floor_isqrt_u64(m);
    idx_t fsq = fs * fs;
    idx_t next = fs + 1;
    idx_t nextsq = next * next;
    idx_t rround = fs;
    if ((m - fsq) > (nextsq - m))
        rround = next;

    idx_t R_val = m + rround;
    idx_t S_val = floor_isqrt_u64(R_val);

    seq_t R = (seq_t)R_val;
    seq_t S = (seq_t)S_val;

    /* partA = R*(R+1)/2 */
    seq_t partA = R * (R + (seq_t)1) / (seq_t)2;

    /* partB = S*(S+1)*(2*S+1)/6 */
    seq_t partB = S * (S + (seq_t)1) * ((seq_t)2 * S + (seq_t)1) / (seq_t)6;

    return partA - partB;
}

seq_t a(idx_t n)
{
    /* T(n) = ceil(n/2) = (n+1)/2 */
    idx_t T_val = (n + (idx_t)1) / (idx_t)2;
    seq_t T = (seq_t)T_val;

    seq_t part1 = T * ((seq_t)1 + T * ((seq_t)3 + (seq_t)2 * T)) / (seq_t)6;
    seq_t part2 = compute_U(n / (idx_t)2);

    return part1 + part2;
}
