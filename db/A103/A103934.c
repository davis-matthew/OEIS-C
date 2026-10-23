/*
 * A103934  Choose a(n) to be the smallest odd number not yet used such that:
 *          a(1) = 1, a(2n) = prime, a(2n+1) = composite and partial sums are
 *          alternately composite or prime.
 * https://oeis.org/A103934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A103934:
   a(1)=1. For n≥2 choose the smallest odd number not yet used such that:
      • a(2k) is prime,
      • a(2k+1) is composite (for k≥1),
      • the partial sums satisfy:
          * s(1)=1,
          * s(n) is composite for even n,
          * s(n) is prime for odd n>1.
   The function supports arbitrary-precision seq_t via the utils library.
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"

/* Static state for the sequence */
static seq_t *seq_cache = NULL;   /* 1‑based sequence values */
static size_t seq_cap   = 0;
static idx_t  max_computed = 0;   /* largest index computed */
static seq_t cumulative = 0;      /* sum up to max_computed */

static unsigned char *used = NULL; /* bitmap of used odd numbers (index = value>>1) */
static size_t used_cap = 0;        /* nb of entries allocated in used[] */

/* Ensure seq_cache has room for at least n+1 elements (1‑based). */
static void ensure_seq(idx_t n)
{
    if ((size_t)n < seq_cap) return;
    size_t new_cap = (size_t)n + 32;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Ensure used[] can index idx (i.e. up to idx+1 entries). */
static void ensure_used_idx(idx_t idx)
{
    if ((size_t)idx < used_cap) return;
    size_t new_cap = ((size_t)idx + 1) * 2;
    used = (unsigned char *)realloc(used, new_cap);
    memset(used + used_cap, 0, new_cap - used_cap);
    used_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n=0 */

    /* first allocation */
    if (!seq_cache) {
        seq_cache = (seq_t *)calloc(1, sizeof(seq_t));
        seq_cap   = 1;
    }

    ensure_seq(n);

    /* initialise on the first call */
    if (max_computed == 0) {
        seq_cache[1] = (seq_t)1;
        used = (unsigned char *)calloc(1, sizeof(unsigned char));
        used_cap = 1;
        used[0] = 1;                       /* value 1 used */
        max_computed = 1;
        cumulative   = 1;
    }

    /* compute new terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        bool need_prime = ((i & 1) == 0);            /* even index --> prime */
        bool need_prime_sum = ((i & 1) == 1 && i > 1);/* odd>1 --> sum must be prime */

        seq_t chosen = 0;

        for (idx_t cand = 3; ; cand += 2) {          /* iterate over odd numbers */
            ensure_used_idx(cand >> 1);
            if (used[cand >> 1]) continue;           /* already used */

            /* candidate must satisfy prime/composite requirement */
            if (need_prime) {
                if (!is_prime((uint64_t)cand)) continue;
            } else {
                if (cand <= 1 || is_prime((uint64_t)cand)) continue; /* not composite */
            }

            /* check the partial sum property */
            seq_t sum = cumulative + (seq_t)cand;
            bool sum_prime = is_prime((uint64_t)sum);

            if (need_prime) {        /* even index: sum must be composite */
                if (sum_prime) continue;
            } else {                 /* odd>1: sum must be prime */
                if (!sum_prime) continue;
            }

            chosen = (seq_t)cand;
            break;
        }

        seq_cache[i] = chosen;
        cumulative  += chosen;

        idx_t used_idx = (idx_t)chosen >> 1;
        ensure_used_idx(used_idx);
        used[used_idx] = 1;

        max_computed = i;
    }

    return seq_cache[n];
}
