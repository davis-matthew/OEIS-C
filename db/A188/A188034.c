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
 * Helpers:   floor_phi_int64, floor_phi_unsigned, floormult_sqrt5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_phi_int64.h"
#include "../utils/floor_phi_unsigned.h"
#include "../utils/floormult_sqrt5.h"

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
