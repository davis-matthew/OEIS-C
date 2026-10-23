/*
 * A189930  b(n) = n + [n*r/s] + [n*t/s]; r=1, s=sin(2*Pi/5), t=cos(2*Pi/5).
 * https://oeis.org/A189930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   b(n) = n + floor(n*csc(2*Pi/5)) + floor(n*cot(2*Pi/5))
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 1’eç: 
   The irrational constants csc(72°)≈1.051462275131833… and
   cot(72°)≈0.324919694131496… are approximated by rational
   numbers α≈αnum/SCALE, β≈βnum/SCALE with
   SCALE = 10^12,
   αnum = 1051462275132,
   βnum = 324919694132.
   The error of this approximation is <0.5/SCALE,
   so for every n>=1  
   0 ≤ n·csc(2·Pi/5) – (n·αnum)/SCALE < n·0.5/SCALE.
   For our use (n≤10000) the error is <5⋅10⁻⁹,
   thus the integer part is unchanged.
*/

#include <stdint.h>

static const unsigned long long SCALE_U64   = 1000000000000ULL;  /* 10^12    */
static const unsigned long long ALPHA_NUM_U64 = 1051462275132ULL; /* ceil( csc*10^12 ) */
static const unsigned long long BETA_NUM_U64  =  324919694132ULL; /* ceil( cot *10^12 ) */

/* Compute b(n) as defined in the OEIS entry. */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;

    /* floor(n * csc) */
    __int128 prod_alpha = (__int128)n_seq * (unsigned long long)ALPHA_NUM_U64;
    seq_t floor_alpha = (seq_t)(prod_alpha / (unsigned long long)SCALE_U64);

    /* floor(n * cot) */
    __int128 prod_beta  = (__int128)n_seq * (unsigned long long)BETA_NUM_U64;
    seq_t floor_beta  = (seq_t)(prod_beta / (unsigned long long)SCALE_U64);

    return n_seq + floor_alpha + floor_beta;
}
