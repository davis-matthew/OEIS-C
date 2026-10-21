/*
 * A073457  Numbers k such that phi(k) = pi(k) + 2.
 * https://oeis.org/A073457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(k) is the largest k such that Euler's totient function φ(k) equals
   the prime counting function π(k) plus 2, i.e., φ(k) = π(k) + 2. */
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of the (finite) solution set. */
    static seq_t solutions[64];
    static size_t sol_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Limit sufficient to include all solutions (known to be ≤210). */
        const idx_t LIMIT = 255;

        /* Allocate working arrays. */
        seq_t *phi = (seq_t *)malloc((LIMIT + 1) * sizeof(seq_t));
        unsigned char *is_prime = (unsigned char *)malloc((LIMIT + 1) * sizeof(unsigned char));
        if (!phi || !is_prime) {
            free(phi); free(is_prime);
            return (seq_t)0;             /* allocation failure */
        }

        /* Initialise phi[i] = i and is_prime[i] = true for i > 1. */
        for (idx_t i = 0; i <= LIMIT; ++i) {
            phi[i] = (seq_t)i;
            is_prime[i] = (i > 1) ? 1 : 0;
        }

        /* Sieve to compute φ and mark primes. */
        for (idx_t p = 2; p <= LIMIT; ++p) {
            if (!is_prime[p]) continue;          /* composite */
            for (idx_t j = p; j <= LIMIT; j += p) {
                phi[j] = phi[j] / (seq_t)p * (seq_t)(p - 1);
            }
            for (idx_t j = p + p; j <= LIMIT; j += p) {
                is_prime[j] = 0;
            }
        }

        /* Scan to collect solutions φ(i) = π(i) + 2. */
        idx_t pi_cnt = 0;
        for (idx_t i = 2; i <= LIMIT; ++i) {
            if (is_prime[i]) ++pi_cnt;
            if (phi[i] == (seq_t)(pi_cnt + 2)) {
                if (sol_len < sizeof(solutions) / sizeof(solutions[0])) {
                    solutions[sol_len++] = (seq_t)i;
                }
            }
        }

        free(phi);
        free(is_prime);
        initialized = true;
    }

    if (n == 0 || n > sol_len) return (seq_t)0;
    return solutions[(size_t)n - 1];
}
