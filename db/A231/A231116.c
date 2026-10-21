/*
 * A231116  Numbers k such that the total number of digits of all the divisors
 *          of k written in base 2 is equal to k.
 * https://oeis.org/A231116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the OEIS sequence A231116.
   Since the sequence is defined by a condition on the binary
   digit‑lengths of the divisors of k, the function performs a
   brute‑force search over k.  A static cache is used to memoise
   the values that have already been found.  The algorithm
   runs in time proportional to the largest k examined (≤ 10⁶
   in practise) and therefore satisfies the 2‑second requirement. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *terms   = NULL;
    static size_t  len    = 0;
    static size_t  cap    = 0;
    /* highest k examined so far */
    static idx_t  last_k  = 0;
    /* safe upper bound for the search – large enough to cover all known terms */
    const idx_t MAX_K = 1000000;

    if (n == 0) return 0;                       /* no element 0 → return 0 */

    /* grow cache as new terms are found */
    if (len == cap) {
        cap = cap ? cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
        if (!tmp) {                              /* allocation failed */
            return 0;
        }
        terms = tmp;
    }

    /* continue searching until we have at least n terms or we reach MAX_K */
    while (len < n && last_k < MAX_K) {
        ++last_k;
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= last_k; ++d) {
            if (last_k % d == 0) {
                sum += (seq_t)bitlen_seq_t((seq_t)d);
                idx_t other = last_k / d;
                if (other != d) {
                    sum += (seq_t)bitlen_seq_t((seq_t)other);
                }
            }
        }
        if (sum == (seq_t)last_k) {
            if (len == cap) {                    /* ensure capacity */
                cap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (!tmp) return 0;
                terms = tmp;
            }
            terms[len++] = (seq_t)last_k;
        }
    }

    /* return the nth term if it exists, otherwise 0 */
    if (n <= len) {
        return terms[n - 1];
    }
    return 0;
}
