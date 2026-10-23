/*
 * A153052  Numbers m such that 2*m + 5 is not a prime.
 * https://oeis.org/A153052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th positive integer m ≥ 1 such that 2*m + 5 is
 *            composite (computed by incremental search)
 * Range:     u8 n <= 161, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t next_candidate = 1;  /* next m to test */

    if (n == 0) {
        return 0;  /* undefined for n = 0 */
    }

    /* generate terms until we have at least n values */
    for (idx_t i = cache_len; i < n; ++i) {
        idx_t m = next_candidate;
        while (true) {
            idx_t val = 2 * m + 5;            /* 2*m + 5 */
            if (!is_prime(val))               /* composite? */
                break;
            ++m;
        }

        seq_t *tmp = (seq_t *)realloc(cache, (i + 1) * sizeof(seq_t));
        if (tmp == NULL) {                    /* allocation failed */
            return 0;
        }
        cache = tmp;
        cache[i] = (seq_t)m;

        next_candidate = m + 1;
        cache_len = i + 1;
    }

    return cache[n - 1];
}
