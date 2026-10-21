/*
 * A052665  a(0)=0, for n >= 1, a(n) = (2^(n-1)-1)*n!.
 * https://oeis.org/A052665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(n-1)-1)*n! with a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helpers pow2 and factorial are provided by the test harness. */
#include "../utils/pow2.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* a(0) = 0 */

    /* 2^(n-1) - 1 as a seq_t */
    seq_t p2 = pow2(n - 1);                  /* computes 2^(n-1) */
    seq_t p2m1 = p2 - 1;                     /* 2^(n-1) - 1 */

    /* n! */
    seq_t fact = factorial(n);

    /* (2^(n-1)-1) * n! */
    return p2m1 * fact;
}
