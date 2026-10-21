/*
 * A074787  Sum of squares of the number of unitary divisors of k from 1 to n.
 * https://oeis.org/A074787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} ud(k)^2 = Sum_{k=1..n} 4^{ω(k)} where ω(k)
 *            is the number of distinct prime factors of k
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;                /* a(0) is undefined */

    size_t N = (size_t)n;
    unsigned char *omega = (unsigned char*)calloc(N + 1, sizeof(unsigned char));
    if (!omega) return 0;               /* allocation failure */

    /* Sieve to compute ω(k) for each 1 ≤ k ≤ N */
    for (size_t p = 2; p <= N; ++p) {
        if (omega[p] == 0) {            /* p is prime */
            for (size_t j = p; j <= N; j += p) {
                ++omega[j];
            }
        }
    }

    seq_t sum = 0;
    for (size_t k = 1; k <= N; ++k) {
        /* term = 4^{ω(k)} */
        seq_t term = big_pow((seq_t)4, (seq_t)omega[k]);
        sum += term;
    }

    free(omega);
    return sum;
}
