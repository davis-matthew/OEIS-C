/*
 * A059991  a(n) = 2^(n-2^ord_2(n)) (or 2^(n-A006519(n))).
 * https://oeis.org/A059991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n - 2^ord_2(n)) (offset 1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  Computes  a(n)  for  n ≥ 1  */
seq_t a(idx_t n)
{
    /* Find the exponent of 2 dividing n (ord_2(n)) */
    idx_t tmp = n;          /* copy of n for manipulation */
    idx_t e   = 0;
    while ((tmp & 1ULL) == 0) {  /* while n is even */
        tmp >>= 1;               /* divide n by 2 */
        ++e;                     /* count the factor 2 */
    }

    /* 2^e as an unsigned integer */
    idx_t pow2 = ((idx_t)1) << e;

    /* Exponent for the result: m = n - 2^e */
    idx_t m = n - pow2;

    /* Compute 2^m as a seq_t value */
    seq_t result = (seq_t)1;
    while (m > 0) {
        result <<= 1;   /* multiply by 2 */
        --m;
    }

    return result;
}
