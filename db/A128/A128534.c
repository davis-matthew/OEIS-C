/*
 * A128534  a(n) = Fibonacci(n)*Lucas(n-1).
 * https://oeis.org/A128534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) with initial values a(0)=0,
 *            a(1)=2, a(2)=1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of OEIS sequence A128534 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;

    seq_t a0 = (seq_t)0;   /* a(n-3)   */
    seq_t a1 = (seq_t)2;   /* a(n-2)   */
    seq_t a2 = (seq_t)1;   /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t a3 = 2 * a2 + 2 * a1 - a0;  /* = a(n)   */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
