/*
 * A109950  Number of partitions of n into parts having in decimal
 *          representation mutually no common digits.
 * https://oeis.org/A109950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of partitions of n into parts with mutually
 *            disjoint decimal digits. The code uses an O(n * 2^10) DP:
 *            dp[sum*1024+mask] counts ways to reach 'sum' using digits
 *            encoded in 'mask'. It processes each digit mask group (at most
 *            one part from that group can be used) and updates the DP in...
 * Range:     u8 n <= 45, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   digit_mask_uint32, seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_malloc.h"
#include "../utils/digit_mask_uint32.h"

typedef struct {
    uint32_t *data;
    size_t sz;
    size_t cap;
} part_list;

/* initialise a dynamic array */
static void part_list_init(part_list *pl) {
    pl->data = NULL;
    pl->sz = 0;
    pl->cap = 0;
}

/* append an element */
static void part_list_push(part_list *pl, uint32_t v) {
    if (pl->sz == pl->cap) {
        size_t newcap = pl->cap ? pl->cap * 2 : 16;
        uint32_t *tmp = (uint32_t *)realloc(pl->data, newcap * sizeof(uint32_t));
        if (!tmp) return; /* allocation failed; ignore this element */
        pl->data = tmp;
        pl->cap = newcap;
    }
    pl->data[pl->sz++] = v;
}

/* free a dynamic array */
static void part_list_free(part_list *pl) {
    free(pl->data);
    pl->data = NULL;
    pl->sz = 0;
    pl->cap = 0;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* n is at least 1 by definition of the sequence */
    size_t N = (size_t)n;                /* for array bounds */
    const size_t MASKS = 1024;            /* 10 bits for digits 0–9 */

    /* allocate DP table: (N+1) sums × MASKS digit masks   */
    size_t dp_size = (N + 1) * MASKS;
    seq_t *dp = seq_malloc(dp_size);
    if (!dp) return 0;                   /* allocation failure → 0 */

    /* zero‑initialize */
    for (size_t i = 0; i < dp_size; ++i) dp[i] = 0;
    dp[0] = 1;                            /* sum 0, mask 0 */

    /* build list of parts per digit mask */
    part_list lists[MASKS];
    for (int m = 0; m < MASKS; ++m) part_list_init(&lists[m]);

    for (uint32_t k = 1; k <= (uint32_t)n; ++k) {
        uint32_t m = digit_mask_uint32(k);
        part_list_push(&lists[m], k);
    }

    /* DP over masks: each mask group may contribute at most one part */
    for (int m = 1; m < MASKS; ++m) {
        if (lists[m].sz == 0) continue;           /* no part with this mask */
        uint32_t *nums = lists[m].data;
        size_t sz_nums = lists[m].sz;

        /* iterate over all source masks U disjoint with m */
        for (int U = 0; U < MASKS; ++U) {
            if (U & m) continue;                  /* not disjoint */
            int newMask = U | m;
            /* iterate over all sums */
            for (size_t s = 0; s <= N; ++s) {
                seq_t cur = dp[s * MASKS + U];
                if (cur == 0) continue;
                /* try each part from this mask */
                for (size_t vi = 0; vi < sz_nums; ++vi) {
                    size_t v = nums[vi];
                    size_t ns = s + v;
                    if (ns > N) continue;
                    dp[ns * MASKS + newMask] += cur;
                }
            }
        }
    }

    /* sum over all masks for sum == N */
    seq_t result = 0;
    for (int m = 0; m < MASKS; ++m)
        result += dp[N * MASKS + m];

    /* free temporary structures */
    for (int m = 0; m < MASKS; ++m) part_list_free(&lists[m]);
    free(dp);

    return result;
}
