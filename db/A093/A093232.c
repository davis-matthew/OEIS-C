/*
 * A093232  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 32.
 * https://oeis.org/A093232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A093232: Largest number whose every length‑n decimal
   substring is distinct and divisible by 32.
   • a(1) = 0
   • a(2) = 964
   • a(3) = 5448000
   • a(4) = 88640000
   • a(n) = (10^n – 32) * 10^n   for n ≥ 5  */

/* The routines seq_pow and seq_mul are provided by the build system.  */
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* special cases for the first four terms */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)964;
    if (n == 3) return (seq_t)5448000;
    if (n == 4) return (seq_t)88640000;

    /* for n ≥ 5:  a(n) = (10^n – 32) * 10^n */
    seq_t pow10 = seq_pow((seq_t)10, n);          /* 10^n */
    seq_t pow10_minus32 = pow10 - (seq_t)32;      /* 10^n – 32 */
    return seq_mul(pow10_minus32, pow10);         /* (10^n – 32) * 10^n */
}
