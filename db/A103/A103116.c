/*
 * A103116  a(n) = Sum_{i=1..n} (n-i+1)*phi(i).
 * https://oeis.org/A103116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} (n-i+1)*phi(i)
 * Range:     u8 n <= 12, u16 n <= 85, u32 n <= 3485, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* External function declaration as required */
seq_t a(idx_t n)
{
    /* Special case: a(0) = 0 */
    if (n == 0) return 0;

    /* Static state for caching prefix sums up to the largest n seen. */
    static uint64_t *phi = NULL;        /* φ[0..max_n] */
    static uint64_t *S   = NULL;        /* prefix sums of φ   */
    static uint64_t *T   = NULL;        /* prefix sums of i*φ */
    static uint64_t  max_n = 0;

    /* Grow tables if needed. */
    if ((uint64_t)n > max_n) {
        uint64_t new_n = (uint64_t)n;
        uint64_t new_size = new_n + 1;

        /* Allocate or grow arrays. */
        phi = (uint64_t*)realloc(phi, new_size * sizeof(uint64_t));
        S   = (uint64_t*)realloc(S,   new_size * sizeof(uint64_t));
        T   = (uint64_t*)realloc(T,   new_size * sizeof(uint64_t));

        /* Initialize φ array to its index value. */
        for (uint64_t i = 2; i <= new_n; ++i) {
            phi[i] = i;
        }
        phi[1] = 1;   /* φ(1) = 1 */
        phi[0] = 0;   /* unused, but defined for safety */

        /* Classic sieve for φ (O(n log log n)). */
        for (uint64_t p = 2; p <= new_n; ++p) {
            if (phi[p] == p) { /* p is prime */
                for (uint64_t j = p; j <= new_n; j += p) {
                    phi[j] -= phi[j] / p;
                }
            }
        }

        /* Build prefix sums. */
        S[0] = 0;
        T[0] = 0;
        for (uint64_t i = 1; i <= new_n; ++i) {
            S[i] = S[i - 1] + phi[i];
            T[i] = T[i - 1] + i * phi[i];
        }

        max_n = new_n;
    }

    /* Compute a(n) using the pre‑computed prefix sums. */
    seq_t sum_phi   = (seq_t)S[(uint64_t)n];
    seq_t sum_i_phi = (seq_t)T[(uint64_t)n];
    seq_t result    = ((seq_t)((uint64_t)n + 1) * sum_phi) - sum_i_phi;
    return result;
}
