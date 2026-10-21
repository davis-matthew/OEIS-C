/*
 * A203814  Number of numbers <= n having as many distinct digits as n has.
 * https://oeis.org/A203814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 458, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   num_distinct_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of integers m with 0 ≤ m ≤ n such that
   the number of distinct decimal digits of m equals that of n. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/num_distinct_digits.h"

/* Maximum decimal digits for idx_t (uint64_t can have at most 20 digits). */
#define MAX_DIGITS 40
/* Number of possible digit masks (10 bits). */
#define MASK_BITS 10
#define MASK_COUNT (1 << MASK_BITS)

/* Static cache for digit‑DP memoisation.  The cache is reused for every
   call; we clear the visited flags before each computation.  It is
   large enough for at most MAX_DIGITS positions. */
static seq_t dp_cache[(MAX_DIGITS + 1) * MASK_COUNT * 2];
static unsigned char vis_cache[(MAX_DIGITS + 1) * MASK_COUNT * 2];

/* Global variables used by the recursive DP helper.  They are set by
   count_exact before the first call to dfs and read by each recursion
   step. */
static int  g_digits[MAX_DIGITS];
static int  g_len;          /* number of digits of the current bound n */
static int  g_target;       /* target number of distinct digits K */

/* --------------------------------------------------------------------
   Helper: Recursively counts numbers from position 'pos' onwards,
   given current digit mask 'mask', whether we have started the number
   ('started'), and whether we are still tight to the upper bound
   ('tight').  Memoisation is used only when tight == 0.
   -------------------------------------------------------------------- */
static seq_t
dfs(int pos, unsigned mask, unsigned started, unsigned tight)
{
    if (pos == g_len) {
        if (!started)          /* number zero – handled separately */
            return (seq_t)0;
        return (__builtin_popcount(mask) == g_target) ? (seq_t)1 : (seq_t)0;
    }

    int limit = tight ? g_digits[pos] : 9;
    seq_t total = 0;

    for (int d = 0; d <= limit; ++d) {
        unsigned nstarted = started || (d != 0);
        unsigned nmask    = mask;
        if (nstarted)
            nmask |= (1u << d);
        unsigned ntight = (tight && d == limit);

        total += dfs(pos + 1, nmask, nstarted, ntight);
    }

    if (!tight) {
        unsigned idx = (pos << MASK_BITS) | (mask << 1) | started;
        if (!vis_cache[idx]) {
            vis_cache[idx] = 1;
            dp_cache[idx]  = total;
        }
    }

    return total;
}

/* --------------------------------------------------------------------
   Helper: Counts numbers m in 1 … n having exactly g_target distinct
   digits.  The digits of n are stored in g_digits/g_len, and the
   result is returned as seq_t.  Zero is not counted here; it is added
   separately in a().
   -------------------------------------------------------------------- */
static seq_t
count_exact(idx_t n, int target)
{
    /* Build digits array in most‑significant‑digit first order. */
    idx_t tmp = n;
    int digits[MAX_DIGITS];
    int len = 0;

    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
        for (int i = 0; i < len; ++i)
            g_digits[i] = digits[len - 1 - i];
    }

    g_len    = len;
    g_target = target;

    memset(vis_cache, 0, sizeof(vis_cache));

    return dfs(0, 0, 0, 1);
}

/* --------------------------------------------------------------------
   Main entry point: a(n)
   -------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0)
        return (seq_t)1;

    int k = num_distinct_digits(n);

    /* Count numbers from 1 to n with exactly k distinct digits. */
    seq_t res = count_exact(n, k);

    /* Include zero if it satisfies the condition. */
    if (k == 1)
        res += (seq_t)1;

    return res;
}
