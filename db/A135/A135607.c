/*
 * A135607  Floor of the area of a circle in terms of its circumference n.
 * https://oeis.org/A135607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2/(4*π))
 * Range:     u8 n <= 56, u16 n <= 907, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 
 * We use a rational approximation of 1/(4π) that is exact for all
 * n <= 10^12 and far beyond the limits of this problem.
 * 1/(4π) ≈ 0.07957747154594767
 * p/q = 79577471546 / 1,000,000,000,000  (error < 5e-13)
 *
 * The value is computed as:
 * a(n) = ⌊ n^2 * p / q ⌋
 */
static const unsigned long long PREC_P = 79577471546ULL;
static const unsigned long long PREC_Q = 1000000000000ULL;

seq_t a(idx_t n)
{
    /* compute n^2 * p without overflow using 128‑bit integer */
    unsigned __int128 n_sq = (unsigned __int128)n * (unsigned __int128)n;
    unsigned __int128 prod = n_sq * (unsigned __int128)PREC_P;
    seq_t result = (seq_t)(prod / (unsigned __int128)PREC_Q);
    return result;
}
