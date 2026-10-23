/*
 * A044280  Numbers k such that the string 3,2 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_32_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A044280 – numbers k such that the string 32 occurs in
 * the base‑9 representation of k but not of k–1.
 * The function returns the n‑th term (1‑based).
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/has_32_base9.h"

seq_t a(idx_t n)
{
    /* Maximum number of stored terms – enough for n up to about 10000 */
    enum { MAX_CACHE = 20000 };

    static seq_t  cache[MAX_CACHE];
    static size_t cache_len  = 0;
    static seq_t  next_k     = (seq_t)1;
    static bool   prev_has32 = false;
    static bool   init       = false;

    /* Lazy initialization */
    if (!init) {
        cache_len  = 0;
        next_k     = (seq_t)1;
        prev_has32 = false;
        init       = true;
    }

    /* Extend cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        bool cur_has32 = has_32_base9(next_k);

        if (cur_has32 && !prev_has32) {
            /* Store the qualifying term */
            if (cache_len < (size_t)MAX_CACHE) {
                cache[cache_len++] = next_k;
            } else {
                /* Exceeded pre‑allocated capacity – stop */
                break;
            }
        }

        prev_has32 = cur_has32;
        next_k = next_k + (seq_t)1;
    }

    /* n is guaranteed to be in range for the intended test set */
    return cache[(size_t)n - 1];
}
