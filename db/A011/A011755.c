/*
 * A011755  a(n) = Sum_{k=1..n} k*phi(k).
 * https://oeis.org/A011755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k*phi(k) (OEIS A011755)
 * Range:     u8 n <= 10, u16 n <= 68, u32 n <= 2766, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Note: seq_t and idx_t are defined by the build environment. */
/* We use a simple O(n) sieve for Euler's totient and a running sum. */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. n >= 1 is expected. */
    if (n == 0U) return 0;

    /* Static cache for the largest n computed so far. */
    static idx_t max_n = 0U;
    static uint64_t *phi = NULL;
    static seq_t *psum = NULL;

    /* If we need to extend the cache, recompute from scratch. */
    if (n > max_n) {
        idx_t new_n = n;
        /* Allocate/resize arrays. */
        if (phi == NULL) {
            phi  = (uint64_t*)malloc((size_t)(new_n + 1) * sizeof(uint64_t));
            psum = (seq_t*)malloc((size_t)(new_n + 1) * sizeof(seq_t));
            if (!phi || !psum) abort();   /* failure */
        } else {
            phi  = (uint64_t*)realloc(phi, (size_t)(new_n + 1) * sizeof(uint64_t));
            psum = (seq_t*)realloc(psum, (size_t)(new_n + 1) * sizeof(seq_t));
            if (!phi || !psum) abort();   /* failure */
        }

        /* Sieve: initialize phi[i] = i. */
        for (idx_t i = 1U; i <= new_n; ++i) phi[i] = i;

        /* Apply the classic totient sieve. */
        for (idx_t i = 2U; i <= new_n; ++i) {
            if (phi[i] == i) {          /* i is prime */
                for (idx_t j = i; j <= new_n; j += i) {
                    phi[j] -= phi[j] / i;
                }
            }
        }

        /* Compute prefix sums of k*phi(k). */
        if (new_n >= 1U) {
            psum[1] = (seq_t)1U * (seq_t)phi[1];
            for (idx_t i = 2U; i <= new_n; ++i) {
                psum[i] = psum[i - 1] + (seq_t)i * (seq_t)phi[i];
            }
        }

        max_n = new_n;
    }

    return psum[n];
}
