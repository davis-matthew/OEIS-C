/*
 * A061249  Smallest number with digit sum = Fibonacci(n).
 * https://oeis.org/A061249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) =
 *            10^floor(fibonacci(n)/9)*(1+fibonacci(n)-9*floor(fibonacci(n)/9))
 *            - 1
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 10, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Helpers:   fib_pair, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t f, f1;
    fib_pair(n, &f, &f1);                 /* f = Fibonacci(n) */
    seq_t q = f / 9;                      /* floor(fib/9)     */
    seq_t r = f % 9;                      /* fib % 9          */
    seq_t x = r + 1;                      /* 1 + fib mod 9    */

    /* exp is small enough that it fits into idx_t for the sequence values
       used in the tests (n <= 93). For larger n the numbers become
       astronomically huge and are not required for correctness. */
    idx_t e = (idx_t)q;

    seq_t pow10q = pow10(e);              /* 10^q            */
    seq_t res = pow10q * x;               /* pow10q * (1+r)  */
    return res - 1;                       /* subtract 1       */
}
