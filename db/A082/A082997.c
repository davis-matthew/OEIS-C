/*
 * A082997  a(n) = card{ x <= n : omega(x) = 2 }.
 * https://oeis.org/A082997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{x ≤ n : ω(x) = 2} (ω = number of distinct prime
 *            factors)
 * Range:     u8 n <= 480, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    idx_t N = n;

    /* Allocate temporary arrays */
    uint8_t *omega = (uint8_t *)malloc((size_t)(N + 1));
    bool   *is_prime = (bool *)malloc((size_t)(N + 1));
    if (!omega || !is_prime) {
        free(omega);
        free(is_prime);
        return 0;
    }

    /* Initialise arrays */
    for (idx_t i = 0; i <= N; ++i) {
        omega[i] = 0;
        is_prime[i] = true;
    }
    is_prime[0] = is_prime[1] = false;

    /* Sieve to count distinct prime factors */
    for (idx_t p = 2; p <= N; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p; m <= N; m += p) {
                ++omega[m];
                if (m > p) is_prime[m] = false;   /* keep p marked as prime */
            }
        }
    }

    /* Count numbers with exactly two distinct prime factors */
    seq_t count = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (omega[i] == 2) ++count;
    }

    /* Clean up */
    free(omega);
    free(is_prime);

    return count;
}
