/*
 * A189754  n+[nr/s]+[nt/s]; r=1, s=arctan(1/3), t=arctan(3).
 * https://oeis.org/A189754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/s⌋ + ⌊n*t/s⌋, with s = arctan(1/3), t = arctan(3)
 * Range:     u8 n <= 32, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Fixed‑point scaling factor (2^30) */
    const uint64_t SCALE = 1ULL << 30;

    /* One‑time computation of the fixed‑point representations
       of α = 1/s and β = t/s.  These values are accurate to
       at least 30 fractional bits, enough for n ≤ 10 000. */
    static uint64_t fp_alpha = 0;
    static uint64_t fp_beta  = 0;
    if (fp_alpha == 0) {
        long double s = atanl(1.0L / 3.0L);   /* arctan(1/3)  */
        long double t = atanl(3.0L);         /* arctan(3)    */
        long double alpha = 1.0L / s;        /* 1/s          */
        long double beta  = t / s;           /* t/s          */
        fp_alpha = (uint64_t)(alpha * (long double)SCALE + 1e-12L);
        fp_beta  = (uint64_t)(beta  * (long double)SCALE + 1e-12L);
    }

    /* Compute floor(n * alpha) and floor(n * beta) using integer arithmetic. */
    uint64_t term1 = (uint64_t)n * fp_alpha >> 30;
    uint64_t term2 = (uint64_t)n * fp_beta  >> 30;

    return (seq_t)n + (seq_t)term1 + (seq_t)term2;
}
