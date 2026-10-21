/*
 * A093240  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 40.
 * https://oeis.org/A093240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=0, a(2)=800, a(n)=10^n * (10^(n-1)-4) for n≥3 a(n) is the
 *            largest number whose length‑n substrings are all distinct and
 *            divisible by 40
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)800;

    seq_t pow_n   = pow10(n);        /* 10^n   */
    seq_t pow_n1  = pow10(n - 1);    /* 10^(n-1) */
    seq_t diff    = pow_n1 - (seq_t)4;
    return pow_n * diff;
}
