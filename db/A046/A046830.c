/*
 * A046830  Digits of n^2 include digits of n as subsequence, n does not end
 *          in 0.
 * https://oeis.org/A046830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-01)
 * Helpers:   get_digits, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A046830
   a(n) = n-th positive integer m (m does not end in 0) such that
   the decimal representation of m^2 contains the decimal representation
   of m as a subsequence. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include <stdint.h>
#include "../utils/get_digits.h"
#include "../utils/num_digits.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Return true iff all digits of “needle” appear in order inside “haystack”. */
static bool contains_subseq(const uint8_t *hay, size_t hlen,
                            const uint8_t *needle, size_t nlen)
{
    size_t h = 0, n = 0;
    while (h < hlen && n < nlen) {
        if (hay[h] == needle[n]) ++n;
        ++h;
    }
    return n == nlen;
}

seq_t a(idx_t n)
{
    /* static memoisation state */
    static idx_t    cached     = 0;   /* number of terms currently cached   */
    static seq_t   *cache      = NULL;/* dynamic array of cached terms      */
    static size_t  cap        = 0;   /* current capacity of cache          */
    static seq_t   candidate  = 1;   /* next number to consider in search  */

    if (n == 0) return 0; /* undefined for n < 1 */

    /* Grow the cache to hold at least n terms */
    if (cached < n) {
        /* Allocate or expand the cache array */
        if (cache == NULL) {
            cap = n > 1 ? n : 1;
            cache = seq_alloc(cap);
        } else if (n > cap) {
            size_t new_cap = cap;
            while (n > new_cap) new_cap <<= 1;
            cache = seq_realloc(cache, cap, new_cap);
            cap = new_cap;
        }

        /* Temporary digit buffers – large enough for all numbers we handle. */
        static uint8_t cand_buf[2048], sq_buf[2048];
        size_t cand_len, sq_len;

        /* Generate terms until the n-th is found */
        while (cached < n) {
            /* Find next candidate satisfying the property */
            while (true) {
                /* Skip numbers ending in 0 */
                if ((candidate % 10u) != 0u) {
                    seq_t sq = candidate * candidate;

                    /* Get lengths of the candidate and its square */
                    cand_len = num_digits(candidate);
                    sq_len   = num_digits(sq);

                    /* Extract digits into the buffers */
                    get_digits(candidate, cand_buf, &cand_len);
                    get_digits(sq, sq_buf, &sq_len);

                    /* Check subsequence property */
                    if (contains_subseq(sq_buf, sq_len, cand_buf, cand_len))
                        break; /* candidate is accepted */
                }
                ++candidate; /* try next number */
            }

            /* Store the accepted candidate */
            cache[cached++] = candidate;
            ++candidate;    /* start next search from the following number */
        }
    }

    return cache[n - 1];
}
