/*
 * A065251  Simple quasi-periodic sequence consisting of the terms 1, 0 and
 *          -1.
 * https://oeis.org/A065251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1-((n-2^[log_2(n)]) mod 3)
 * Range:     i8 n <= 110, i16 n <= 110, i32 n <= 110, i64 n <= 110,
 *            i128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for the OEIS sequence A065251. 
   idx_t is an unsigned integer type (uint64_t). 
   seq_t is a signed integer type (e.g. int8_t .. __int128). */
seq_t a(idx_t n)
{
    /* n >= 1, so __builtin_clzll is defined */
    unsigned k = 63 - __builtin_clzll(n);          /* floor(log2(n))     */
    seq_t pow2_mod3 = (k & 1) ? 2 : 1;              /* 2^k mod 3          */
    seq_t n_mod3   = (seq_t)(n % 3);                /* n mod 3             */
    seq_t diff_mod3 = n_mod3 - pow2_mod3;           /* (n - 2^k) mod 3    */
    if (diff_mod3 < 0)
        diff_mod3 += 3;                            /* ensure non‑negative */
    return 1 - diff_mod3;                          /* final answer       */
}
