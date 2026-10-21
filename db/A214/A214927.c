/*
 * A214927  Number of n-digit numbers N that do not end with 0 and are such
 *          that the reversal of N divides N but is different from N.
 * https://oeis.org/A214927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 49, u32 n <= 95, u64 n <= 187,
 *            u128 n <= 371, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2 * Fibonacci(floor((n-2)/2))   for n > 1,
 * a(1) = 0
 *
 * Here Fibonacci is the classical sequence F(0)=0, F(1)=1,
 * F(k+1)=F(k)+F(k-1).  This function works for any seq_t type
 * (uint8_t … arbitrary‑precision) and handles all n ≥ 1.
 */
#include <stdbool.h>   /* for completeness, not strictly required */

seq_t a(idx_t n)
{
    /* The formula is undefined for n < 1 in the OEIS definition, 
     * but we are only called with n ≥ 1.  For n == 1 the
     * index (n-2)/2 would underflow if we simply subtracted 2
     * from an unsigned integer, so handle it explicitly. */
    if (n < 2)          /* covers n == 1 */
        return 0;

    /* k = floor((n-2)/2) */
    idx_t k = (n - 2) / 2;

    /* Compute F(k) for the classical Fibonacci sequence.
     * F(0)=0, F(1)=1.  We only need the value for k, not the whole array. */
    seq_t f0 = 0;   /* F(0) */
    seq_t f1 = 1;   /* F(1) */

    if (k == 0)     /* F(0) = 0 -> result 0 */
        return 0;

    /* Iterate from 2 to k, computing Fibonacci numbers incrementally. */
    for (idx_t i = 2; i <= k; ++i) {
        seq_t tmp = f0 + f1;  /* F(i) = F(i-2) + F(i-1) */
        f0 = f1;
        f1 = tmp;
    }

    /* a(n) = 2 * F(k) */
    return f1 + f1;
}
