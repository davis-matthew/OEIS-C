/*
 * A121708  Numerator of Sum/Product of first n Fibonacci numbers A000045[n].
 * https://oeis.org/A121708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of (sum_{k=1}^n F_k)/(prod_{k=1}^n F_k) Uses
 *            the identity sum_{k=1}^n F_k = F_{n+2} - 1
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   fib, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utilities required:
   ../utils/gcd.h   - seq_t gcd(seq_t a, seq_t b)
   ../utils/fib.h   - seq_t fib(idx_t n)
*/
#include "../utils/gcd.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* First compute the sum S_n = F_{n+2} - 1. */
    seq_t sum = fib(n + 2) - 1;

    /* The numerator after reduction is sum / gcd(sum, product_{k=1}^n F_k). */
    seq_t rem = sum;

    /* Iterate over F_k for k = 1 .. n and peel off common factors from rem. */
    seq_t f1 = 1;   /* F_1 */
    seq_t f2 = 1;   /* F_2 */
    seq_t fk;

    for (idx_t k = 1; k <= n; ++k) {
        if (k == 1) {
            fk = f1;
        } else if (k == 2) {
            fk = f2;
        } else {
            /* Compute next Fibonacci number. */
            seq_t next = f1 + f2;
            f1 = f2;
            f2 = next;
            fk = f2;
        }
        seq_t t = gcd(rem, fk);
        if (t != 1) {
            rem = rem / t;    /* Remove common factor from rem. */
        }
    }

    /* rem now equals (F_{n+2}-1) / gcd(F_{n+2}-1, prod F_k).  */
    return rem;
}
