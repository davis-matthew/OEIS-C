/*
 * A063234  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          82 ).
 * https://oeis.org/A063234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A063234: Dimension of the space of weight 2n cuspidal newforms for Gamma_0( 82 ).
   Recurrence used:  a(1)=3,
   for n≥2  a(n)=a(n‑1)+d(n) where
       d(2)=7,         d(3)=8,
       d(n)=4 if (n-4)%3==0,
       d(n)=8 otherwise.  (Pattern of differences 7,8,4,8,8,4,8,8,4,…)
   This reproduces all listed terms (and the entire b‑file). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first term directly */
    if (n == 1) {
        return (seq_t)3;
    }

    seq_t val = (seq_t)3;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t d;
        if (i == 2) {
            d = (seq_t)7;
        } else if (i == 3) {
            d = (seq_t)8;
        } else {
            idx_t offset = i - 4;               /* offset ≥ 0 */
            if ((offset % 3) == 0) {
                d = (seq_t)4;
            } else {
                d = (seq_t)8;
            }
        }
        val += d;
    }

    return val;
}
