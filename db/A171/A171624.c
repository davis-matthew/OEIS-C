/*
 * A171624  Floor(n-th nonprime / n).
 * https://oeis.org/A171624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(A018252(n)/n) (n-th nonprime divided by n)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   grow_seq_array, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;        /* not defined for n < 1, return 0 */

    /* static cache of computed n‑th nonprimes (1‑based indexing) */
    static seq_t   *cache   = NULL;   /* cache[n] holds the n‑th nonprime */
    static size_t  cap     = 0;        /* current allocated capacity   */
    static idx_t   last    = 0;        /* last number inspected       */
    static idx_t   count   = 0;        /* number of nonprimes found   */

    /* Ensure the cache can hold at least indices up to n.  */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
        grow_seq_array(&cache, &cap, new_cap);
    }

    /* Grow the cache to at least n: compute further nonprimes.  */
    while (count < n) {
        last++;                               /* next integer to test   */
        if (!is_prime(last)) {                /* 1 is not prime, so is a nonprime */
            count++;                          /* found another nonprime */
            cache[count] = (seq_t)last;       /* store it in the cache  */
        }
    }

    /* Return floor(third expression) = integer division */
    return cache[n] / (seq_t)n;
}
