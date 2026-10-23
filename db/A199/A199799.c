/*
 * A199799  Totatives of 111111.
 * https://oeis.org/A199799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = n‑th integer coprime to 111111
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    const idx_t N = 111111ULL;   /* 3*7*11*13*37 */

    /* Build cache on first call */
    if (cache == NULL) {
        /* Compute φ(N) = 51840 */
        idx_t primes[] = {3, 7, 11, 13, 37};
        const int pcount = (int)(sizeof(primes) / sizeof(primes[0]));
        idx_t phi = N;
        for (int i = 0; i < pcount; ++i) {
            phi = (phi / primes[i]) * (primes[i] - 1);
        }
        cache_len = phi;

        /* Allocate array for totatives */
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failed */

        idx_t idx = 0;
        for (idx_t i = 1; i < N; ++i) {
            if (gcd((seq_t)i, (seq_t)N) == (seq_t)1) {
                cache[idx++] = (seq_t)i;
            }
        }
        /* Safety check: ensure we stored the expected number of terms */
        if (idx != cache_len) cache_len = idx;
    }

    /* Return the n‑th term (1‑based index) */
    if (n < 1 || n > cache_len) return (seq_t)0;
    return cache[n - 1];
}
