/*
 * A034968  Minimal number of factorials that add to n.
 * https://oeis.org/A034968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the digits of n when written in factorial base
 *            (A007623). The function computes the factorial-base digits of n
 *            and returns their sum
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sum   = 0;
    seq_t fact  = 1;          /* (k)! */
    idx_t k     = 1;

    /* Find largest k such that k! <= n */
    while (fact <= (seq_t)n) {
        k++;
        fact *= (seq_t)k;     /* k! */
    }
    /* k! is now the first factorial > n.
       Reduce to (k-1)! which is <= n. */
    fact /= (seq_t)k;
    k--;

    seq_t rem = (seq_t)n;

    /* Decompose rem in factorial base and sum digits. */
    while (k >= 1) {
        seq_t digit = rem / fact;      /* digit at position k */
        rem  -= digit * fact;
        sum += digit;

        fact /= (seq_t)k;              /* (k-1)! -> (k-2)! */
        k--;
    }

    return sum;
}
