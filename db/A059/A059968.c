/*
 * A059968  Number of 10-ary trees.
 * https://oeis.org/A059968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(10*n, n)/(9*n+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   legendre
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1; for n>=1 this yields the numbers of 10‑ary trees. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/legendre.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t M = 10 * n;           /* 10*n */
    idx_t D = 9 * n + 1;        /* (9*n+1) */

    /* Sieve primes up to M (M <= 10*n).  For n up to 10000, M is <= 100000. */
    size_t limit = (size_t)M;
    bool *is_composite = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_composite) return (seq_t)0;  /* allocation failure */

    for (idx_t i = 2; i * i <= M; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= M; j += i)
                is_composite[j] = true;
        }
    }

    seq_t result = 1;
    for (idx_t p = 2; p <= M; ++p) {
        if (is_composite[p]) continue;  /* skip non‑primes */

        idx_t e = legendre(M, p) - legendre(D, p) - legendre(n, p);
        for (idx_t k = 0; k < e; ++k)
            result *= (seq_t)p;
    }

    free(is_composite);
    return result;
}
