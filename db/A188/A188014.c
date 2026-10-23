/*
 * A188014  a(n) = [nr]-[nr-kr]-[kr], where r=(1+sqrt(5))/2, k=4, [ ]=floor.
 * https://oeis.org/A188014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n r] - [n r - 4 r] - [4 r], where r = (1+sqrt(5))/2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Beatty approximation p/q < r = (1+sqrt(5))/2.
   Use the convergent 987/610 which is below φ and has error < 1/(q*q). */
#define PHI_P 987ULL
#define PHI_Q 610ULL
#define PHI_Q2 (PHI_Q * PHI_Q)

/* Compute floor(n * φ) for non‑negative n.
   Uses the rational approximation p/q < φ and a small correction
   to obtain the exact integer value. */
static inline int64_t floor_phi_uint(uint64_t n)
{
    /* compute floor((n * p) / q) and the remainder */
    unsigned __int128 prod = (unsigned __int128)n * PHI_P;
    uint64_t div = (uint64_t)(prod / PHI_Q);
    uint64_t rem = (uint64_t)(prod % PHI_Q);

    /* Decide whether the result should be increased by 1.
       Condition derived from: floor(n φ) = div      if rem * q < q² - n
                                            = div + 1  otherwise  */
    uint64_t remq = rem * PHI_Q;
    if (remq >= PHI_Q2 - n) {
        div += 1;
    }
    return (int64_t)div;
}

/* Compute floor(m * φ) for signed m.
   For negative m, floor(m φ) = -ceil(|m| φ) = -(floor(|m| φ) + 1)      */
static inline int64_t floor_phi(int64_t m)
{
    if (m >= 0) {
        return floor_phi_uint((uint64_t)m);
    } else {
        uint64_t abs_m = (uint64_t)(-m);
        int64_t f = floor_phi_uint(abs_m);
        return -(f + 1);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1, but we treat it as signed for the shift (n-4) */
    int64_t fn = floor_phi((int64_t)n);
    int64_t fm = floor_phi((int64_t)n - 4);
    int64_t val = fn - fm - 6;          /* floor(4φ) = 6 */
    return (seq_t)val;                  /* val is 0 or 1 */
}
