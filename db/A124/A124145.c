/*
 * A124145  a(1)=1, a(2)=2, a(n)=smallest number greater than a(n-1) that can
 *          be written as sum of consecutive earlier terms in exactly one way.
 * https://oeis.org/A124145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=2; a(n)=smallest integer > a(n-1) that can be
 *            written as sum of consecutive earlier terms in exactly one way
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 81 terms through n = 955 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static inline int count_exact_subarray_sum(const seq_t *arr, idx_t len,
                                           seq_t target)
{
    int cnt = 0;
    seq_t sum = 0;
    idx_t left = 1;
    for (idx_t right = 1; right <= len; right++) {
        sum += arr[right];
        while (sum > target && left <= right) {
            sum -= arr[left];
            left++;
        }
        if (sum == target) {
            cnt++;
            if (cnt > 1)
                return cnt;
        }
    }
    return cnt;
}

static inline void grow_seq(idx_t *len, idx_t *cap, seq_t **seq, idx_t needed)
{
    if (needed <= *cap)
        return;
    idx_t new_cap = *cap ? *cap << 1 : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    *seq = mem_resize(seq_t, *seq, new_cap);
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Static storage of the sequence up to the largest previously computed index */
    static seq_t *seq = NULL;      /* 1‑based array, seq[0] unused */
    static idx_t len = 0;          /* number of stored terms (seq[1..len]) */
    static idx_t cap = 0;          /* current capacity */

    /* initialise with first two terms */
    if (len == 0) {
        grow_seq(&len, &cap, &seq, 4);   /* allocate at least 4 elements */
        seq[1] = 1;
        seq[2] = 2;
        len = 2;
    }

    /* if the term has already been computed, return it */
    if (n <= len)
        return seq[n];

    /* compute terms up to n */
    while (len < n) {
        seq_t candidate = seq[len] + 1;   /* start searching after previous term */

        /* search the next term */
        while (1) {
            int cnt = count_exact_subarray_sum(seq, len, candidate);
            if (cnt == 1) {               /* unique representation found */
                len++;
                grow_seq(&len, &cap, &seq, len + 1);   /* make room for new term */
                seq[len] = candidate;
                break;
            }
            candidate++;                    /* try next integer */
        }
    }

    return seq[n];
}
