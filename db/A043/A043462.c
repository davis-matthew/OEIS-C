/*
 * A043462  Numbers having two 2's in base 9.
 * https://oeis.org/A043462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A043462: the nth integer whose base‑9 representation
   contains exactly two digits equal to 2.  The function generates the
   sequence by searching successive integers. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously found terms */
    static seq_t *cache      = NULL; /* cache[0] = a(1), cache[1] = a(2), ... */
    static size_t cache_len  = 0;    /* number of cached terms        */
    static size_t cache_cap  = 0;    /* capacity of the cache array   */
    static idx_t next_candidate = 20; /* first integer with two 2's in base 9 */

    /* grow and fill the cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* ensure sufficient capacity */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* allocation fail – fallback */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* search for the next integer with exactly two 2's in base‑9 */
        while (1) {
            idx_t t = next_candidate;
            int twos = 0;
            while (t) {
                if ((t % 9) == 2) {
                    ++twos;
                    if (twos > 2) break;
                }
                t /= 9;
            }
            if (twos == 2) {
                cache[cache_len++] = (seq_t)next_candidate;
                ++next_candidate;                 /* move to next integer */
                break;
            }
            ++next_candidate;                     /* try next integer */
        }
    }
    return cache[(size_t)n - 1];
}
