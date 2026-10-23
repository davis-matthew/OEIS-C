/*
 * A044080  Numbers k such that string 0,3 occurs in the base 5 representation
 *          of k but not of k-1.
 * https://oeis.org/A044080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044080: numbers k such that base‑5 representation of k contains the
   substring 0,3 but the representation of k‑1 does not.  The function
   generates the sequence on demand and memoises the results. */

#include <stdlib.h>
#include <stdbool.h>

/* Check if the base‑5 representation of x contains the adjacent digits
   0 followed by 3. */
static inline bool has_03(seq_t x)
{
    if (x == 0)
        return false;                      /* representation "0" */

    unsigned digits[64];
    size_t len = 0;

    /* Extract base‑5 digits (least significant first). */
    while (x > 0) {
        digits[len++] = (unsigned)(x % 5);
        x /= 5;
    }

    /* Scan from most significant to least significant digit. */
    for (size_t i = len - 1; i > 0; --i) {
        if (digits[i]     == 0 &&
            digits[i - 1] == 3)
            return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Memoised sequence generation.                                         */
/* ------------------------------------------------------------------ */
static seq_t   *cache     = NULL;   /* memoised sequence values   */
static size_t   cache_cap = 0;      /* allocated capacity         */
static size_t   cache_len = 0;      /* number of stored terms     */
static seq_t    last_k    = 0;      /* last k processed           */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1; return 0 as a fallback */
        return 0;

    /* Ensure the cache contains at least n terms. */
    while (cache_len < n) {
        /* Find the next qualifying k. */
        seq_t cand = last_k + 1;
        while (1) {
            if (has_03(cand) && !has_03(cand - 1)) {
                /* Ensure capacity. */
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* allocation failure – abort. */
                        exit(EXIT_FAILURE);
                    }
                    cache     = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cand;
                last_k = cand;
                break;
            }
            /* Move to the next candidate. */
            ++cand;
        }
    }

    return cache[n - 1];
}
