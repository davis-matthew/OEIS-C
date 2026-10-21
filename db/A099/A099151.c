/*
 * A099151  Positive integers a such that f(3a)+f(a)=concatenation of 3a and
 *          a, where f(k)=k(k+3)/2 (A000096).
 * https://oeis.org/A099151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*10^(n-1) - 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 5, a(2) = 59, a(3) = 599, … */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* The definition is valid for n ≥ 1. For n = 0 the
       function returns 0, which is never requested. */
    if (n == 0) return 0;

    seq_t pow10_val = pow10(n - 1);   /* compute 10^(n-1) */
    return 6 * pow10_val - 1;         /* 6 * 10^(n-1) - 1 */
}
