/*
 * A116828  Number of permutations of length n which avoid the patterns 2413,
 *          3421, 4123.
 * https://oeis.org/A116828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 10*a(n-1) -
 *            42*a(n-2) + 100*a(n-3) * -150*a(n-4) + 144*a(n-5) - 86*a(n-6) *
 *            + 27*a(n-7) - 2*a(n-8) - a(n-9) for n ≥ 10 * with a(1)=1,
 *            a(2)=2, ..., a(9)=8511. * This is derived from the rational
 *            generating function given in the OEIS entry
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   compute_ranks, equal_ranks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_ranks.h"
#include "../utils/equal_ranks.h"

/* ------------------------------------------------------------ */
/*  Helpers for pattern avoidance (used only for n ≤ 9)          */
/* ------------------------------------------------------------ */
/* Checks whether the permutation p[0..n-1] contains any of the
 * forbidden patterns: 2413, 3421, 4123.  p holds distinct
 * integers in the range [0, n-1].  n ≤ 9.
 */
static const int pat2413[4] = {1, 3, 0, 2};
static const int pat3421[4] = {2, 3, 1, 0};
static const int pat4123[4] = {3, 0, 1, 2};

static bool contains_pattern(const int *p, int n) {
    for (int a = 0; a < n - 3; ++a)
        for (int b = a + 1; b < n - 2; ++b)
            for (int c = b + 1; c < n - 1; ++c)
                for (int d = c + 1; d < n; ++d) {
                    int v[4] = {p[a], p[b], p[c], p[d]};
                    int rank[4];
                    compute_ranks(v, rank);
                    if (equal_ranks(rank, pat2413) ||
                        equal_ranks(rank, pat3421) ||
                        equal_ranks(rank, pat4123))
                        return true;
                }
    return false;
}

/* Generic next_permutation implementation (works in C and C++). */
static bool next_permutation(int *seq, int n) {
    int i = n - 2;
    while (i >= 0 && seq[i] > seq[i + 1]) --i;
    if (i < 0) return false;
    int j = n - 1;
    while (seq[j] < seq[i]) --j;
    int t = seq[i]; seq[i] = seq[j]; seq[j] = t;
    for (int l = i + 1, r = n - 1; l < r; ++l, --r) {
        t = seq[l]; seq[l] = seq[r]; seq[r] = t;
    }
    return true;
}

/* Counts permutations of length n (≤9) that avoid the three patterns. */
static seq_t count_avoiding_small(idx_t n) {
    int perm[9];
    seq_t count = 0;
    for (int i = 0; i < (int)n; ++i) perm[i] = i;
    do {
        if (!contains_pattern(perm, (int)n)) ++count;
    } while (next_permutation(perm, (int)n));
    return count;
}

/* ------------------------------------------------------------ */
/*  Memoised sequence computation                               */
/* ------------------------------------------------------------ */
static seq_t *cache = NULL;       /* dynamically grown cache */
static size_t cache_cap = 0;
static size_t cache_len = 0;      /* number of values currently stored */

static void ensure_cache_size(size_t n) {
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < n) new_cap = n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In a proper environment this would be handled more gracefully. */
        exit(1);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Fills the cache with the first nine terms by brute force. */
static void initialise_small_terms(void) {
    if (cache_len >= 9) return;
    ensure_cache_size(9);
    for (idx_t i = cache_len; i < 9; ++i) {
        cache[i] = count_avoiding_small(i + 1);
    }
    if (cache_len < 9) cache_len = 9;
}

/* Computes terms using the linear recurrence for n ≥ 10. */
static void compute_up_to(idx_t n) {
    if (n <= cache_len) return;
    ensure_cache_size(n);
    for (idx_t i = cache_len; i < n; ++i) {
        /* i is 0‑based for cache; compute a(i+1) */
        seq_t val =
            10 * cache[i - 1]
          - 42 * cache[i - 2]
          + 100 * cache[i - 3]
          - 150 * cache[i - 4]
          + 144 * cache[i - 5]
          - 86 * cache[i - 6]
          + 27 * cache[i - 7]
          - 2  * cache[i - 8]
          - cache[i - 9];
        cache[i] = val;
    }
    cache_len = n;
}

/* ------------------------------------------------------------ */
/*  Public sequence function                                     */
/* ------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for calls with n=0 */
    ensure_cache_size(9);          /* ensure buffer for seeds */
    initialise_small_terms();      /* small terms via brute force */
    if (n > cache_len)             /* compute additional terms if needed */
        compute_up_to(n);
    return cache[n - 1];            /* cache is 0‑based */
}
