/*
 * A188215  Starting with an empty list, n is inserted after the a(n)th
 *          element such that the binary representations of the list's
 *          elements are always sorted lexicographically.
 * https://oeis.org/A188215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of elements in the lexicographically ordered list
 *            that are smaller than n, where the list is updated by inserting
 *            n after element a(n). The list is kept sorted by binary
 *            representation
 * Range:     u8 n <= 382, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/floor_log2_uint64.h"

/* Lexicographic comparison of binary strings of a and b.
   Return <0 if a<b, 0 if equal, >0 if a>b. */
static inline int cmp_bin(idx_t a, idx_t b)
{
    if (a == 0 && b == 0) return 0;
    if (a == 0) return -1;          /* "0" < any string starting with '1' */
    if (b == 0) return 1;

    /* bit length of a and b */
    idx_t la = floor_log2_uint64(a) + 1;
    idx_t lb = floor_log2_uint64(b) + 1;

    /* compare bits from most‑significant to least‑significant */
    for (idx_t i = 0; i < (idx_t)(la > lb ? la : lb); ++i) {
        int bit_a = (i < la) ? (int)((a >> (la - 1 - i)) & 1) : -1;
        int bit_b = (i < lb) ? (int)((b >> (lb - 1 - i)) & 1) : -1;
        if (bit_a == -1) return -1;   /* a finished → a<b */
        if (bit_b == -1) return 1;    /* b finished → a>b */
        if (bit_a < bit_b) return -1;
        if (bit_a > bit_b) return 1;
    }
    return 0;
}

/* Grow array *arr (type void *) to hold at least *cap_needed elements.
   cap stores the current capacity. */
static void *grow(void **arr, size_t *cap, size_t elem_size, size_t cap_needed)
{
    if (*cap >= cap_needed) return *arr;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < cap_needed) new_cap <<= 1;
    *arr = realloc(*arr, new_cap * elem_size);
    *cap = new_cap;
    return *arr;
}

seq_t a(idx_t n)
{
    /* static memoization data */
    static seq_t *answers = NULL;
    static size_t ans_cap = 0;
    static idx_t  max_computed = (idx_t)-1;   /* sentinel */

    /* sorted list of numbers < current max_computed+1 */
    static idx_t *ordered = NULL;
    static size_t ord_len = 0, ord_cap = 0;

    /* if we already have the value, return it */
    if (max_computed != (idx_t)-1 && n <= max_computed)
        return answers[n];

    /* ensure the answers array is large enough */
    if (answers == NULL) answers = NULL;
    answers = (seq_t *)grow((void **)&answers, &ans_cap, sizeof(seq_t), (size_t)n + 1);

    /* ensure ordered array capacity for at most n+1 elements */
    if (ordered == NULL) ordered = NULL;
    ordered = (idx_t *)grow((void **)&ordered, &ord_cap, sizeof(idx_t), (size_t)n + 1);

    /* compute values from max_computed+1 up to n */
    for (idx_t k = (max_computed == (idx_t)-1 ? 0 : max_computed + 1); k <= n; ++k) {
        /* find insertion position in the sorted list */
        size_t lo = 0, hi = ord_len;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (cmp_bin(ordered[mid], k) < 0)
                lo = mid + 1;
            else
                hi = mid;
        }
        size_t pos = lo;                         /* number of elements < k */

        answers[k] = (seq_t)pos;                 /* a(k) */

        /* insert k into ordered at position pos */
        if (ord_len >= ord_cap) {
            ordered = (idx_t *)grow((void **)&ordered, &ord_cap,
                                     sizeof(idx_t), ord_cap * 2);
        }
        if (pos < ord_len) {
            memmove(&ordered[pos + 1], &ordered[pos],
                    (ord_len - pos) * sizeof(idx_t));
        }
        ordered[pos] = k;
        ++ord_len;
    }

    max_computed = n;
    return answers[n];
}
