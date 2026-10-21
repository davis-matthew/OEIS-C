/*
 * A088669  Number of partitions of n into decimal repdigit numbers.
 * https://oeis.org/A088669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of partitions of n into decimal repdigit numbers
 *            (i.e. numbers of the form d·111…1 with 1≤d≤9). The code uses a
 *            standard coin‑change DP to count unordered partitions
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 208, u64 n <= 1275,
 *            u128 n <= 10000, big any n
 * Helpers:   xcalloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/xcalloc.h"

/* Helper: realloc that returns a typed pointer (needed for C++ compilation). */
static void* xrealloc(void *ptr, size_t sz)
{
    return realloc(ptr, sz);
}

seq_t a(idx_t n)
{
    /* The empty partition of 0 is counted once. */
    if (n == 0)
        return (seq_t)1;

    /* Generate all repdigit part sizes ≤ n. */
    idx_t *parts = NULL;
    size_t part_cap = 0;
    size_t part_len = 0;

    unsigned __int128 repunit = 0;          /* current repunit 1, 11, 111, ... */
    while (1) {
        unsigned __int128 next = repunit * 10 + 1;
        if (next > (unsigned __int128)n || next > (unsigned __int128)UINT64_MAX)
            break;
        repunit = next;

        for (idx_t d = 1; d <= 9; ++d) {
            unsigned __int128 val = repunit * d;
            if (val > (unsigned __int128)n)
                continue;
            if (part_len == part_cap) {
                part_cap = part_cap ? part_cap * 2 : 8;
                parts = (idx_t *)xrealloc(parts, part_cap * sizeof(idx_t));
            }
            parts[part_len++] = (idx_t)val;
        }
    }

    /* DP array: dp[j] = number of partitions of j. */
    size_t dp_size = (size_t)n + 1;
    seq_t *dp = (seq_t *)xcalloc(dp_size, sizeof(seq_t));
    dp[0] = (seq_t)1;

    for (size_t i = 0; i < part_len; ++i) {
        idx_t p = parts[i];
        for (idx_t j = p; j <= n; ++j)
            dp[j] += dp[j - p];
    }

    seq_t result = dp[n];
    free(dp);
    free(parts);
    return result;
}
