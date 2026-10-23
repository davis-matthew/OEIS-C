/*
 * A026625  a(n) = Sum_{k=0..floor(n/2)} A026615(n-k,k).
 * https://oeis.org/A026625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n-1) + Lucas(n) - (3 + (-1)^n)/2 for n>1; a(0)
 *            = 1, a(1) = 1
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utilities */
#include "../utils/fib.h"

/* Compute the sequence A026625 */
seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Fibonacci(n-1) */
    seq_t fib_nm1 = fib(n - 1);

    /* Lucas(n) */
    seq_t lucas_n;
    if (n == 0) {
        lucas_n = (seq_t)2;
    } else if (n == 1) {
        lucas_n = (seq_t)1;
    } else {
        seq_t l0 = (seq_t)2; /* L(0) */
        seq_t l1 = (seq_t)1; /* L(1) */
        seq_t lcurr;
        for (idx_t i = 2; i <= n; ++i) {
            lcurr = l0 + l1;
            l0 = l1;
            l1 = lcurr;
        }
        lucas_n = l1;
    }

    /* Subtract (3 + (-1)^n)/2 which is 2 for even n, 1 for odd n */
    seq_t sub = (n % 2 == 0) ? (seq_t)2 : (seq_t)1;

    return fib_nm1 + lucas_n - sub;
}
