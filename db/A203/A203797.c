/*
 * A203797  a(n) = Pell(n) * Sum_{d|n} 1/Pell(d), where Pell(n) = A000129(n).
 * https://oeis.org/A203797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Pell(n) * Sum_{d|n} 1/Pell(d) (Pell(0)=0, Pell(1)=1,
 *            Pell(k)=2*Pell(k-1)+Pell(k-2))
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; guard for zero */
        return (seq_t)0;

    /* Allocate array to store Pell numbers up to n */
    seq_t *P = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!P)
        abort();

    P[0] = (seq_t)0;
    P[1] = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        P[i] = (seq_t)2 * P[i - 1] + P[i - 2];

    seq_t sum = (seq_t)0;
    seq_t Pn  = P[n];

    /* Iterate over divisors of n without overflow */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            sum += Pn / P[d];          /* contribution of divisor d */
            idx_t d2 = n / d;
            if (d2 != d)
                sum += Pn / P[d2];      /* contribution of paired divisor */
        }
    }

    free(P);
    return sum;
}
