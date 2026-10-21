/*
 * A219651  a(n) = n minus (sum of digits in factorial base expansion of n).
 * https://oeis.org/A219651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - (sum of digits in factorial base expansion of n)
 * Range:     u8 n <= 262, u16 n <= 9595, u32 n <= 9595, u64 n <= 9595,
 *            u128 n <= 9595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return (seq_t)0;

    /* Convert the index to the generic integer type */
    seq_t nn = (seq_t)n;

    /* Find the largest k such that k! <= nn.
       f holds k! after each iteration. */
    seq_t f = 1;
    idx_t k = 1;
    while (1) {
        /* If f already exceeds nn, stop */
        if (f > nn)
            break;
        /* If multiplying by (k+1) would exceed nn (or overflow),
           then f is the largest factorial <= nn. */
        if (f > nn / (k + 1))
            break;
        f *= (k + 1);
        ++k;
    }
    idx_t high = k;            /* high! = f, 1 <= high <= k */

    /* Compute the sum of the factorial base digits */
    seq_t sum = 0;
    for (idx_t i = high; i >= 1; --i) {
        seq_t digit = nn / f;
        nn -= digit * f;
        sum += digit;
        f /= i; /* f is divisible by i */
    }

    return (seq_t)n - sum;
}
