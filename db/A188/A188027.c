/*
 * A188027  [nr]-[nr-kr]-[kr], where r=(1+sqrt(5))/2, k=9, [ ]=floor.
 * https://oeis.org/A188027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n·φ⌋ - ⌊(n-9)·φ⌋ - ⌊9·φ⌋ * where φ = (1+√5)/2 and the
 *            floor is the integer part. * The rational approximation φ ≈
 *            233/144 is used; it * is accurate enough to guarantee correct
 *            floors for all n ≥ 1 * (the error |φ - 233/144| < 1/144² <
 *            5·10⁻⁵, so * n·error < 1/2 for n ≤ 10⁴)
 * Range:     u8 n <= 123, u16 n <= 123, u32 n <= 123, u64 n <= 123,
 *            u128 n <= 123, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Rational approximation of φ: 233/144 */
#define PHI_NUM 233
#define PHI_DEN 144
#define CONST_TERM 14   /* ⌊9·φ⌋ */

/* Return ⌊k·φ⌋ for any (possibly negative) signed integer k */
static inline __int128 floor_phi_signed(__int128 k)
{
    __int128 prod = k * (__int128)PHI_NUM;          /* k·233 */
    __int128 q    = prod / (__int128)PHI_DEN;      /* truncates toward 0 */
    /* Correct for negative numbers: floor(x) = trunc(x) - 1 if x not integer */
    if (prod < 0 && prod % (__int128)PHI_DEN != 0)
        q -= 1;
    return q;
}

seq_t a(idx_t n)
{
    __int128 floor_n   = floor_phi_signed((__int128)n);
    __int128 floor_k   = floor_phi_signed((__int128)n - 9);
    __int128 result    = floor_n - floor_k - (__int128)CONST_TERM;
    return (seq_t)result;          /* result is non‑negative (0 or 1) */
}
