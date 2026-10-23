/*
 * A171934  Backwards van Eck transform of A000010.
 * https://oeis.org/A171934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - prev_index_of_phi(n) if φ(n) has appeared before,
 *            otherwise a(n) = 0. This is the Backwards van Eck transform of
 *            Euler's totient sequence (A000010)
 * Range:     u8 n <= 525, u16 n <= 9250, u32 n <= 9250, u64 n <= 9250,
 *            u128 n <= 9250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1, but return 0 for safety */

    /* Allocate array for φ(1..n).  φ(n) ≤ n‑1 for n > 1, so idx_t fits. */
    idx_t *phi = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!phi)
        return 0;   /* allocation failure, return 0 */

    /* Initialise φ[i] = i. */
    for (idx_t i = 0; i <= n; ++i)
        phi[i] = i;

    /* Sieve of Eratosthenes to compute φ for all i ≤ n. */
    for (idx_t i = 2; i <= n; ++i) {
        if (phi[i] == i) {           /* i is prime */
            for (idx_t j = i; j <= n; j += i)
                phi[j] -= phi[j] / i;
        }
    }

    /* lastOcc[v] holds the last index where φ‑value v was seen. */
    idx_t *lastOcc = (idx_t*)calloc(n + 1, sizeof(idx_t));
    if (!lastOcc) {
        free(phi);
        return 0;
    }

    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t val = phi[i];
        idx_t prev = lastOcc[val];
        result = (prev != 0) ? (seq_t)(i - prev) : (seq_t)0;
        lastOcc[val] = i;
    }

    free(phi);
    free(lastOcc);
    return result;
}
