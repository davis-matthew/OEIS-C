/*
 * A188034  [nr]-[nr-kr]-[kr], where r=(1+sqrt(5))/2, k=12, [ ]=floor.
 * https://oeis.org/A188034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n·φ⌋ – ⌊(n–12)·φ⌋ – ⌊12·φ⌋ where φ = (1+√5)/2 and ⌊12·φ⌋
 *            = 19. The implementation uses only integer arithmetic; it is
 *            O(1) per call and works for all n ≥ 1
 * Range:     u8 n <= 133, u16 n <= 133, u32 n <= 133, u64 n <= 133,
 *            u128 n <= 133, big any n
 * Validated: partial, pass, 133 terms through n = 133 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ----------------------------------------------------------------------
 *  Return ⌊|k|·√5⌋ for 0 ≤ k ≤ 2^64−1.
 *  Uses a binary search on the 128‑bit value 5·k².
 * ---------------------------------------------------------------------- */
static inline uint64_t floormult_sqrt5(uint64_t k)
{
    __uint128_t target = (__uint128_t)5 * k * k;
    uint64_t low  = 0;
    uint64_t high = (uint64_t)(((__uint128_t)k * 3 + 1) > (__uint128_t)UINT64_MAX
                               ? UINT64_MAX
                               : (__uint128_t)k * 3 + 1);
    while (low < high) {
        uint64_t mid = low + ((high - low + 1) >> 1);
        __uint128_t mid2 = (__uint128_t)mid * mid;
        if (mid2 <= target)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ----------------------------------------------------------------------
 *  Compute floor(k·φ) for signed 64‑bit integer k.
 *  Uses the identity ⌊k·φ⌋ = ⌊(k + ⌊k·√5⌋)/2⌋ with proper handling
 *  of negative arguments.
 * ---------------------------------------------------------------------- */
static inline int64_t floor_phi_int64(int64_t k)
{
    uint64_t k_abs = k >= 0 ? (uint64_t)k : (uint64_t)(-k);
    uint64_t sqrt5_mul_k = floormult_sqrt5(k_abs);
    int64_t floor_sqrt5;
    if (k >= 0) {
        floor_sqrt5 = (int64_t)sqrt5_mul_k;
    } else {
        /* floor(k·√5) = –(floor(|k|·√5) + 1) */
        floor_sqrt5 = -( (int64_t)sqrt5_mul_k + 1 );
    }
    int64_t num = k + floor_sqrt5;              /* may be negative */
    int64_t q   = num / 2;                      /* truncates toward 0 */
    if (num < 0 && (num & 1LL)) q -= 1;         /* adjust to floor */
    return q;
}

/* ----------------------------------------------------------------------
 *  Compute floor(k·φ) for unsigned 64‑bit integer k (k ≥ 0).
 *  Uses the same identity but with unsigned arithmetic; no
 *  adjustment needed since the result is non‑negative.
 * ---------------------------------------------------------------------- */
static inline seq_t floor_phi_unsigned(uint64_t k)
{
    uint64_t sqrt5_mul_k = floormult_sqrt5(k);
    __uint128_t sum      = (__uint128_t)k + sqrt5_mul_k; /* non‑negative   */
    seq_t result = (seq_t)(sum >> 1);                    /* floor(sum/2) */
    return result;
}

/* ----------------------------------------------------------------------
 *  Integer square root for 64‑bit values (floor of √x).
 *  Uses binary search, works for all 0 ≤ x < 2^64.
 * ---------------------------------------------------------------------- */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 1ULL << 32;    /* hi^2 = 2^64 >= x */
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (mid > 0 && (uint64_t)mid > x / mid)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo - 1;
}

/* ----------------------------------------------------------------------
 *  OEIS sequence A188034.
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;               /* n is at least 1            */
    int64_t  fp_n = (int64_t)floor_phi_unsigned(nn);

    int64_t fp_n_minus12;
    if (n <= 12UL)                         /* negative argument needed   */
        fp_n_minus12 = floor_phi_int64((int64_t)n - 12);
    else
        fp_n_minus12 = (int64_t)floor_phi_unsigned(nn - 12);

    /* ⌊12·φ⌋ = 19 follows from the defining formula. */
    int64_t val = fp_n - fp_n_minus12 - 19;
    return (seq_t)val;                      /* 0 or 1 for all n ≥ 1      */
}
