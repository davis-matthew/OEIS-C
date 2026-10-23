/*
 * A140845  Primes of the form 210k + 29.
 * https://oeis.org/A140845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form 210k + 29 (k ≥ 0)
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 0;   /* next candidate index to test */

    if (n == 0)
        return 0;              /* undefined for offset 0, just return 0 */

    /* generate terms until we have at least n primes */
    while (cache_len < n) {
        /* ensure room for one more element */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return 0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* candidate of the form 210k + 29 */
        seq_t cand = (seq_t)210 * ((seq_t)next_k) + (seq_t)29;
        next_k++;                      /* advance for next iteration */

        if (is_prime_big(cand)) {       /* primality test */
            cache[cache_len++] = cand;
        }
    }

    /* return the cached term (offset 1 -> index 0) */
    return cache[n - 1];
}
