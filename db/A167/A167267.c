/*
 * A167267  Interspersion of the signature sequence of (1+sqrt(5))/2.
 * https://oeis.org/A167267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For index n (offset 1) let (i,j) be the pair of positive
 *            integers obtained by reading the infinite array of joint ranks
 *            of the numbers i + j*φ (φ = (1+√5)/2) along antidiagonals:
 *            (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),… . Then a(n) is the rank of
 *            i + j*φ among all such values, which can be computed...
 * Range:     u8 n <= 153, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------------------------------------------------------------------
 *  Integer square root (floor) of a 128‑bit unsigned integer.
 * ------------------------------------------------------------------ */
static inline uint64_t isqrt_u128(__int128 x)
{
    /* binary search on [0, 2^63) because (2^63)^2 < 2^127 */
    __int128 low = 0;
    __int128 high = (__int128)1 << 63;
    while (high - low > 1) {
        __int128 mid = (low + high) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return (uint64_t)low;
}

/* ------------------------------------------------------------------
 *  floor(k*φ) for any integer k, returned as a signed 128‑bit value.
 *  Uses the identity φ = 1 + (φ−1) and (φ−1) = (√5−1)/2.
 * ------------------------------------------------------------------ */
static inline __int128 floor_phi_signed(int64_t k)
{
    if (k >= 0) {
        __int128 kk = (__int128)k;
        __int128 val = (__int128)5 * kk * kk;      /* 5*k*k  */
        uint64_t sqrt_val = isqrt_u128(val);       /* floor(√(5*k*k)) */
        __int128 diff = (__int128)sqrt_val - kk;   /* ≥0 */
        __int128 add  = diff / 2;
        return kk + add;
    } else {
        __int128 pos = floor_phi_signed(-k);
        return - (pos + (__int128)1);
    }
}

/* ------------------------------------------------------------------
 *  Compute the rank of i + j*φ for positive i,j.
 *  Uses the formula above.
 * ------------------------------------------------------------------ */
static inline seq_t rank_of_pair(idx_t i, idx_t j)
{
    /* Y = j + floor((i-1)/φ) = j + floor((i-1)*φ) - (i-1) */
    __int128 cf = floor_phi_signed((int64_t)(i - 1));   /* floor((i-1)*φ) ≥ 0 */
    __int128 Y128 = (__int128)j + cf - (__int128)(i - 1);
    seq_t Y = (seq_t)Y128;                              /* Y fits into seq_t */

    __int128 rank128 = (__int128)i * Y128;

    int64_t k_start = (int64_t)j - (int64_t)Y128;
    int64_t k_end   = (int64_t)j - 1;
    for (int64_t k = k_start; k <= k_end; ++k)
        rank128 += floor_phi_signed(k);

    return (seq_t)rank128;
}

/* ------------------------------------------------------------------
 *  Main function: a(n)   (offset 1)
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* 1. Find the antidiagonal index s such that
       (s-1)(s-2)/2 < n ≤ (s-1)s/2. */
    __int128 disc = (__int128)1 + (__int128)8 * n;   /* 1+8n */
    uint64_t sqrt_d = isqrt_u128(disc);
    uint64_t s = (sqrt_d + 1) / 2;

    while (( (__int128)(s - 1) * s) / 2 < n) ++s;
    while (s >= 2 && (((__int128)(s - 2) * (s - 1)) / 2) >= n) --s;

    /* 2. Lexicographic coordinates (i,j) within this diagonal */
    uint64_t first = ( (__int128)(s - 2) * (s - 1) ) / 2 + 1;
    uint64_t offset = n - first;               /* 0‑based offset in diagonal */
    uint64_t i = 1 + offset;
    uint64_t j = (s - 1) - offset;              /* j ≥ 1 */

    /* 3. Compute rank which is the desired term */
    return rank_of_pair(i, j);
}
