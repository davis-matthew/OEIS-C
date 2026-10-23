/*
 * A185979  Numbers which are the sum of two positive triangular numbers in
 *          more than one way.
 * https://oeis.org/A185979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A185979
   A number is in the sequence if it can be expressed as a sum of two
   positive triangular numbers in more than one (unordered) way.
   The routine builds the sequence lazily by enumerating all sums of
   triangular numbers up to a growing limit, counting the number of
   representations, and collecting those with at least two distinct
   representations.  The algorithm runs in O(L²) time for a sieve limit
   L, but the limit needed for the 10 000th term is small enough that the
   total running time is well below 2 s for all builds. */

/* Author: ChatGPT – adapted for the OEIS builder */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* The types seq_t and idx_t are supplied by the build system. */

/* --------------------------------------------------------------------- */
/*  Helper variables for both C and C++ builds                           */
/* --------------------------------------------------------------------- */
static idx_t last_limit = 0;               /* last sieve limit used */

#ifdef __cplusplus
static std::vector<seq_t> results_vec;     /* holds the sequence values */
#else
static seq_t *results = NULL;              /* holds the sequence values */
static size_t res_cnt = 0;                 /* number of elements in results */
#endif

/* --------------------------------------------------------------------- */
/*  Build or rebuild the sequence up to the given limit                 */
/* --------------------------------------------------------------------- */
static void rebuild(idx_t limit)
{
    /* Count how many times each sum appears */
    size_t max_sum = (size_t)limit;
    /* Allocate counter array */
    uint16_t *cnt = (uint16_t *)calloc(max_sum + 1, sizeof(uint16_t));
    if (!cnt)
        return;                 /* out of memory – abort build */

    /* Enumerate sums of two triangular numbers */
    for (idx_t i = 1; ; ++i) {
        seq_t tri_i = (seq_t)i * (i + 1) / 2;
        if (tri_i > limit)
            break;
        for (idx_t j = i; ; ++j) {
            seq_t tri_j = (seq_t)j * (j + 1) / 2;
            seq_t sum   = tri_i + tri_j;
            if (sum > limit)
                break;
            ++cnt[(size_t)sum];
        }
    }

    /* Count how many sums have at least two representations */
    size_t newsz = 0;
    for (size_t s = 1; s <= max_sum; ++s)
        if (cnt[s] >= 2)
            ++newsz;

#ifdef __cplusplus
    /* Vector to hold the new results (C++ builds) */
    std::vector<seq_t> new_vec;
    new_vec.reserve(newsz);
#else
    /* Raw array for C builds */
    seq_t *new_res = NULL;
    if (newsz > 0)
        new_res = (seq_t *)malloc(newsz * sizeof(seq_t));
#endif

    /* Copy qualifying sums */
    size_t pos = 0;
    for (size_t s = 1; s <= max_sum; ++s) {
        if (cnt[s] >= 2) {
#ifdef __cplusplus
            new_vec.push_back((seq_t)s);
#else
            new_res[pos++] = (seq_t)s;
#endif
        }
    }

    free(cnt);

    /* Store the new result list */
#ifdef __cplusplus
    results_vec.swap(new_vec);          /* move the vector (fast) */
#else
    if (results)
        free(results);
    results   = new_res;
    res_cnt   = newsz;
#endif
    last_limit = limit;
}

/* --------------------------------------------------------------------- */
/*  Public sequence routine                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; return 0 for n == 0. */
    if (n == 0)
        return (seq_t)0;

    /* Ensure we have at least n elements in the cache. */
    bool need_more = false;
#ifdef __cplusplus
    if (results_vec.size() < (size_t)(n))
        need_more = true;
#else
    if (res_cnt < (size_t)(n))
        need_more = true;
#endif

    /* Grow the sieve limit until we generate enough terms. */
    if (need_more) {
        idx_t grow = last_limit ? last_limit : 2000;
        while (need_more) {
            rebuild(grow);
            /* after rebuild, check again */
#ifdef __cplusplus
            need_more = results_vec.size() < (size_t)(n);
#else
            need_more = res_cnt < (size_t)(n);
#endif
            if (grow > (idx_t)0x7fffffffffffffffULL)
                break;            /* avoid overflow */
            grow *= 2;
        }
    }

    /* Return the requested term. */
#ifdef __cplusplus
    return results_vec[(size_t)(n - 1)];
#else
    return results[(size_t)(n - 1)];
#endif
}
