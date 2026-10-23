/*
 * A173552  Numbers k such that 5+38*k^2 is a prime.
 * https://oeis.org/A173552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th integer k such that 5 + 38*k^2 is prime
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 29, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* allow any order of calls; keep cache of found terms */
    static idx_t *cache = NULL;     /* stored k-values */
    static idx_t   size  = 0;       /* number of terms cached */
    static size_t cap  = 0;         /* allocated capacity */

    /* generate terms until at least n are available */
    if (size < n) {
        idx_t next_k = (size == 0) ? 0 : cache[size - 1] + 1;
        while (size < n) {
            /* 5 + 38*k^2; use 128‑bit arithmetic to avoid overflow in
               intermediate stages before truncation to seq_t           */
            unsigned __int128 val128 = 5ULL;
            val128 += (unsigned __int128)38 * next_k * next_k;
            seq_t cand = (seq_t)val128;

            if (is_prime_big(cand)) {       /* prime? */
                /* expand cache if necessary */
                if (size == cap) {
                    size_t new_cap = (cap == 0) ? 16 : cap << 1;
                    idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
                    if (!tmp) exit(1);
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[size++] = next_k;       /* store the found k */
            }
            next_k++;                         /* test next integer */
        }
    }
    return (seq_t)cache[n - 1];
}
