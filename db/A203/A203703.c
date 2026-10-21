/*
 * A203703  Vandermonde sequence using x^2 + xy + y^2 applied to the first n
 *          primes.
 * https://oeis.org/A203703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1 ≤ i < j ≤ n} (p_i^2 + p_i·p_j + p_j^2), where p_k is
 *            the k‑th prime. a(1) = 1 (empty product)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* below offset – undefined */
    uint32_t *primes = NULL;
    size_t cnt = 0;
    sieve_first_n_primes(n, &primes, &cnt);   /* primes[0..cnt-1] are the first n primes */

    if (cnt != (size_t)n) {              /* safety check */
        free(primes);
        return 0;
    }

    seq_t result = 1;
    for (size_t i = 0; i < (size_t)n; ++i) {
        seq_t pi = (seq_t)primes[i];
        seq_t pi_sq = pi * pi;
        for (size_t j = i + 1; j < (size_t)n; ++j) {
            seq_t pj = (seq_t)primes[j];
            seq_t val = pi_sq + pi * pj + pj * pj;
            result *= val;
        }
    }

    free(primes);
    return result;
}
