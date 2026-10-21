/*
 * A061662  Numbers k > 1 such that, in base 8, k and k^2 contain the same
 *          digits in the same proportion.
 * https://oeis.org/A061662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_digits128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers k > 1 such that in base 8, k and k² contain the same digits
    in the same proportion (each digit's multiplicity in k² is twice that
    in k).  The function a(idx_t n) searches for the n‑th such number,
    caching found terms for future calls.  It works for any seq_t
    integer type (uint8_t … uint64_t, unsigned __int128, or arbitrary
    precision).  The search stops when the list contains the required
    index n. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_digits128.h"

static seq_t *term_cache = NULL;          /* cached terms */
static size_t term_count  = 0;            /* number of cached terms */
static size_t term_cap    = 0;            /* allocated capacity */
static unsigned long long next_k = 2;     /* next k to test */

/* grow the cache if needed */
static void ensure_cache(size_t needed)
{
    if (needed <= term_cap) return;
    size_t new_cap = term_cap ? term_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    term_cache = tmp;
    term_cap = new_cap;
}

static bool ok_candidate(unsigned long long k)
{
    /* digit counts of k */
    unsigned char cnt_k[8] = {0};
    unsigned long long tmp = k;
    while (tmp > 0) {
        ++cnt_k[tmp & 7];
        tmp >>= 3;
    }

    /* digit counts of k² */
    unsigned __int128 sq = (unsigned __int128)k * (unsigned __int128)k;
    unsigned char cnt_sq[8];
    count_digits128(sq, cnt_sq);

    for (int d = 0; d < 8; ++d) {
        if (cnt_k[d] == 0) {
            if (cnt_sq[d] != 0) return false;         /* absent digit must be absent */
        } else {
            if (cnt_sq[d] != cnt_k[d] * 2) return false; /* ratio must be 2 for all digits */
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1; value for n==0 is irrelevant */

    if (term_count < n) {
        ensure_cache(n);
        while (term_count < n) {
            unsigned long long k = next_k++;
            if (k <= 1) continue;
            if (ok_candidate(k)) {
                term_cache[term_count++] = (seq_t)k;
            }
        }
    }
    return term_cache[n - 1];
}
