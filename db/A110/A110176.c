/*
 * A110176  Least k such that sigma(n) = sigma(k) + sigma(n-k) for 0<k<n, or 0
 *          if there is no such k, where sigma is the sum of divisors
 *          function.
 * https://oeis.org/A110176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k (0<k<n) with σ(n) = σ(k)+σ(n-k), where σ is the
 *            sum‑of‑divisors function, 0 if none
 * Range:     u8 n <= 581, u16 n <= 9786, u32 n <= 9786, u64 n <= 9786,
 *            u128 n <= 9786, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for all builds (uint*_t or arbitrary‑precision seq_t)
   by storing σ in a 64‑bit array, which is enough for n up to ~10⁵.   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static state allows calls in any order. */
    static idx_t      max_n = 0;          /* highest n processed so far   */
    static uint64_t * sigma = NULL;       /* σ values for 0 … max_n        */

    if (n == 0) return (seq_t)0;          /* safety – caller guarantees n>=1 */

    /* Reallocate and recompute σ if a larger n is requested. */
    if (n > max_n) {
        /* Allocate space for 0 … n (index 0 unused). */
        uint64_t *tmp = (uint64_t *)realloc(sigma, (n + 1) * sizeof(uint64_t));
        if (!tmp) return (seq_t)0;        /* allocation failure → safe 0 */
        sigma = tmp;

        /* Initialise to zero. */
        for (idx_t i = 0; i <= n; ++i) sigma[i] = 0;

        /* Classic harmonic sieve for σ. */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i) {
                sigma[j] += (uint64_t)i;
            }
        }

        max_n = n;
    }

    /* Search for the smallest k with σ(k)+σ(n−k)=σ(n). */
    uint64_t target = sigma[n];
    for (idx_t k = 1; k < n; ++k) {
        if (sigma[k] + sigma[n - k] == target) {
            return (seq_t)k;            /* found the least k */
        }
    }

    /* No such k exists. */
    return (seq_t)0;
}
