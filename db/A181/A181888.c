/*
 * A181888  Second column of triangle in A182971.
 * https://oeis.org/A181888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(5*n+12)*(n+2)*(n+1)/360
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A181888.  The implementation uses a simple
   cancellation-by-gcd strategy to keep intermediate values from
   overflowing any built-in unsigned type.  It works for all
   supported seq_t sizes and for arbitrary‑precision builds. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compute each factor as seq_t.  All arithmetic on n is done
       in idx_t (uint64_t) first, then cast to seq_t.  n>=1 is
       guaranteed by the caller. */
    seq_t f[6];
    f[0] = (seq_t)n;
    f[1] = (seq_t)(n - 1);
    f[2] = (seq_t)(n - 2);
    f[3] = (seq_t)(5 * n + 12);
    f[4] = (seq_t)(n + 2);
    f[5] = (seq_t)(n + 1);

    seq_t result = 1;
    seq_t div = 360;           /* 2^3 * 3^2 * 5 */

    for (size_t i = 0; i < 6; ++i) {
        result *= f[i];

        /* Remove common factors of the current result with the
           remaining divisor 'div'.  This keeps the product small. */
        while (div > 1) {
            /* Euclidean algorithm for gcd(result, div) */
            seq_t a = result;
            seq_t b = div;
            while (b != 0) {
                seq_t t = a % b;
                a = b;
                b = t;
            }
            seq_t g = a;          /* gcd */
            if (g == 1)
                break;
            result /= g;
            div /= g;
        }
    }
    /* After cancelling all factors, div should be 1. */
    return result;
}
