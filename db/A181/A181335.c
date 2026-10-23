/*
 * A181335  Partial products of A036691.
 * https://oeis.org/A181335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=0..n} ( Product_{j=1..i} A002808(j) ) (where
 *            A002808(j) is the j-th composite number)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

/* Computes the n-th term of OEIS sequence A181335. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;        /* a(0) = 1 */

    seq_t compProd = (seq_t)1;  /* A036691(i) for current i       */
    seq_t result   = (seq_t)1;  /* a(n) = product of compositorials */
    idx_t cur = 4;              /* next integer to test for composite */

    for (idx_t i = 1; i <= n; ++i) {
        /* Locate the next composite number. */
        while (is_prime(cur))
            ++cur;

        /* Update the compositorial and overall product. */
        compProd *= (seq_t)cur;
        result   *= compProd;

        ++cur;                  /* move past the used composite */
    }

    return result;
}
