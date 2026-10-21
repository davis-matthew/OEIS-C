/*
 * A074166  Product of first n palindromes.
 * https://oeis.org/A074166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = product of the first n base‑10 palindromes (10 is not
 *            considered palindrome). The implementation generates palindromes
 *            in order, multiplies them and caches intermediate products so
 *            that any call a(n) (for n≥1) is answered in O(1) after an
 *            initial linear pass
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 17, u128 n <= 26,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* malloc, realloc */
#include <stdbool.h>     /* bool */
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cached       = NULL;   /* a(1), a(2), … */
    static size_t cached_len   = 0;      /* number of cached products */
    static size_t cached_cap   = 0;      /* allocated capacity     */
    static idx_t   processed   = 0;      /* how many palindromes processed */
    static idx_t   next_candidate = 1;   /* next integer to test   */
    static seq_t  prod          = 1;      /* product of processed palindromes */

    if (n == 0) return 0;                /* undefined for offset 0 */

    while (processed < n) {
        seq_t cand = (seq_t)next_candidate;
        if (is_palindrome(cand, 10)) {
            prod *= cand;                /* update running product    */
            processed++;

            /* grow cache if necessary */
            if (cached_len == cached_cap) {
                size_t new_cap = cached_cap ? cached_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failure – return current product
                       (implementation fails gracefully) */
                    return prod;
                }
                cached = tmp;
                cached_cap = new_cap;
            }
            cached[cached_len++] = prod;  /* store a(processed)      */
        }
        ++next_candidate;
    }

    return cached[n - 1];
}
