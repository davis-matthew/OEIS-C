/*
 * A184916  n+[sn/r]+[tn/r]+[un/r], where []=floor and r=1, s=2^(1/4), t=s^2,
 *          u=s^3.
 * https://oeis.org/A184916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(2^(1/4)*n) + floor(2^(1/2)*n) +
 *            floor(2^(3/4)*n)
 * Range:     u8 n <= 48, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Scale factors for the irrational constants, multiplied by 2^32 and floored.
   The constants are computed at compile time using double precision, which
   is safe because the resulting product is less than 2^33, and the rounding
   error is far below 1. */
static const uint64_t SCALE_S = (uint64_t)(1.1892071150027210664713852755096635476033018 * (double)(1ULL << 32));
static const uint64_t SCALE_T = (uint64_t)(1.4142135623730950488016887242096980785696718753769480731766797 * (double)(1ULL << 32));
static const uint64_t SCALE_U = (uint64_t)(1.6817928305074290551965394939003330724412715457786427778082015 * (double)(1ULL << 32));

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* the sequence is defined for n >= 1, but return 0 for safety */

    /* Compute each floor value using 128-bit multiplication and a right shift.
       (value * 2^32) >> 32 == value when value = floor(k * 2^32) and k = sqrt constant. */
    unsigned __int128 prod_s = (unsigned __int128)SCALE_S * n;
    unsigned __int128 prod_t = (unsigned __int128)SCALE_T * n;
    unsigned __int128 prod_u = (unsigned __int128)SCALE_U * n;

    seq_t term_s = (seq_t)(prod_s >> 32);   /* floor(2^(1/4) * n)  */
    seq_t term_t = (seq_t)(prod_t >> 32);   /* floor(2^(1/2) * n)  */
    seq_t term_u = (seq_t)(prod_u >> 32);   /* floor(2^(3/4) * n)  */

    return (seq_t)n + term_s + term_t + term_u;
}
