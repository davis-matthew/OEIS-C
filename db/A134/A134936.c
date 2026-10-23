/*
 * A134936  Numbers that are the sum of a nonzero pentagonal number and a
 *          nonzero square in at exactly three ways.
 * https://oeis.org/A134936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers that are the sum of a nonzero pentagonal number and a nonzero square
   in exactly three distinct ways.  The function a(n) returns the n‑th such number
   (a(1) = 71, a(2) = 86, …).  The implementation builds a cache of the first
   terms large enough for the requested n and expands it on demand.  All
   arithmetic uses the user’s seq_t type, and size_t/idx_t are used for
   array indices.  No external libraries are required except the standard
   headers below. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *seq_cache = NULL;     /* sorted list of terms found */
static size_t seq_cache_len = 0;    /* number of cached terms          */
static bool   seq_cache_init = false; /* has the cache been built? */

/* Extend the cache until it contains at least target_terms elements. */
static void build_cache(size_t target_terms)
{
    idx_t N = 500000;   /* initial search limit */
    seq_t *tmp = NULL;
    size_t tmp_cap = 0, tmp_len = 0;

    while (1) {
        /* Count occurrences up to N.  Use unsigned char to avoid overflow. */
        unsigned char *cnt = (unsigned char *)malloc((size_t)(N + 1) * sizeof(unsigned char));
        if (!cnt) exit(1);
        memset(cnt, 0, (size_t)(N + 1) * sizeof(unsigned char));

        /* Generate all sums p + s^2 <= N.  p = k(3k-1)/2,  s,k >= 1. */
        for (idx_t k = 1; ; ++k) {
            seq_t p = (seq_t)k * (3 * k - 1) / 2;
            if (p > (seq_t)N) break;
            for (idx_t s = 1; ; ++s) {
                idx_t sq = s * s;
                seq_t sum = p + (seq_t)sq;
                if ((idx_t)sum > N) break;   /* sum fits in idx_t */
                unsigned char c = cnt[(idx_t)sum];
                if (c < 4) cnt[(idx_t)sum] = c + 1;
            }
        }

        /* Collect numbers with exactly 3 representations. */
        for (idx_t num = 1; num <= N; ++num) {
            if (cnt[(idx_t)num] == 3) {
                if (tmp_len >= tmp_cap) {
                    size_t new_cap = tmp_cap ? tmp_cap * 2 : 1024;
                    seq_t *new_buf = (seq_t *)realloc(tmp, new_cap * sizeof(seq_t));
                    if (!new_buf) exit(1);
                    tmp = new_buf;
                    tmp_cap = new_cap;
                }
                tmp[tmp_len++] = (seq_t)num;
            }
        }

        free(cnt);

        if (tmp_len >= target_terms) {
            seq_cache = (seq_t *)malloc(tmp_len * sizeof(seq_t));
            if (!seq_cache) exit(1);
            memcpy(seq_cache, tmp, tmp_len * sizeof(seq_t));
            seq_cache_len = tmp_len;
            free(tmp);
            seq_cache_init = true;
            break;
        }

        /* Need more terms: enlarge N and try again. */
        free(tmp);
        tmp = NULL;
        tmp_len = 0;
        tmp_cap = 0;
        N *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (!seq_cache_init) {
        /* Precompute a decent number of terms. */
        build_cache(20000);
    }

    /* If requested n exceeds cached terms, extend the cache. */
    if ((size_t)n > seq_cache_len) {
        /* Ask for twice the needed size to reduce future allocations. */
        size_t new_target = (size_t)n * 2;
        build_cache(new_target);
    }

    return seq_cache[n - 1];
}
