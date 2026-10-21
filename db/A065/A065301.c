/*
 * A065301  Numbers k such that both k and the sum of its divisors are
 *          squarefree numbers.
 * https://oeis.org/A065301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 2450, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_squarefree, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate numbers k such that k and sigma(k) are squarefree
 * by scanning integers and storing the first n valid terms. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/sigma_of.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t      *cache = NULL;  /* cached terms */
    static size_t      cache_len = 0; /* number of cached terms */
    static size_t      cache_cap = 0; /* allocated capacity */
    static idx_t       next_i  = 1;   /* next integer to test */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        idx_t i = next_i++;                  /* current integer to test   */
        if (is_squarefree( (seq_t)i )) {
            seq_t sigma = sigma_of(i);       /* sum of divisors of i      */
            if (is_squarefree(sigma)) {
                grow_seq( &cache, &cache_cap, cache_len + 1 );
                cache[cache_len++] = (seq_t)i;
            }
        }
    }
    return cache[n-1];
}
