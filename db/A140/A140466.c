/*
 * A140466  a(n) = 4*A002088(n).
 * https://oeis.org/A140466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * sum_{k=1}^n phi(k)
 * Range:     u8 n <= 13, u16 n <= 231, u32 n <= 9956, u64 n <= 9956,
 *            u128 n <= 9956, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Allocate array for Euler's totient values up to n */
    seq_t *phi = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!phi)
        return 0;   /* allocation failure – return 0 as a safe fallback */

    /* Initialise phi[i] = i */
    phi[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        phi[i] = (seq_t)i;

    /* Compute totients using a simple sieve */
    for (idx_t i = 2; i <= n; ++i)
    {
        if (phi[i] == (seq_t)i)          /* i is prime */
        {
            for (idx_t j = i; j <= n; j += i)
                phi[j] -= phi[j] / (seq_t)i;
        }
    }

    /* Sum the totients */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
        sum += phi[i];

    /* Multiply by 4 to obtain the number of distinct slopes */
    seq_t result = sum * ((seq_t)4);

    free(phi);
    return result;
}
