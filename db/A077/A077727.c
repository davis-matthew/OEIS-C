/*
 * A077727  Smallest multiple of n beginning with n and having a digit sum n,
 *          or 0 if no such number exists.
 * https://oeis.org/A077727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   Compute the smallest multiple of n that begins with n and * has
 *            a digit sum equal to n; return 0 if no such multiple exists
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   sum_digits_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sum_digits_idx_t.h"

/* Global data used by the recursive search. */
#define MAX_P 25                          /* maximum suffix length tried   */
#define MAX_N 10000                       /* maximum possible index n       */
#define MAX_NS (9 * MAX_P)                /* maximum possible digit sum     */
#define VISIT_TOTAL (((size_t)MAX_P + 1) * MAX_N * (MAX_NS + 1))

static unsigned char *visit_arr = NULL;   /* visited marker array          */
static unsigned char visit_tag = 1;       /* current search ID             */
static int g_n = 0;                      /* current n (as int)            */
static int g_need = 0;                   /* required digit sum of suffix  */
static int g_p = 0;                      /* current suffix length         */
static unsigned char g_digits[MAX_P];     /* digits of found suffix        */

/* Recursive DFS that tries to build the suffix of length g_p. 
 * 'pos'  : current position in the suffix (0 .. g_p)
 * 'rem'  : current remainder of the suffix modulo g_n
 * 'sum'  : current sum of digits in the suffix
 * Returns true if a valid suffix is found; on success g_digits array
 * holds the digits in order from most significant to least. */
static bool dfs(int pos, int rem, int sum)
{
    if (pos == g_p) {
        return (sum == g_need && rem == 0);
    }

    /* Prune if even using all 9's we cannot reach the required sum. */
    int remaining = g_p - pos;
    if (sum + 9 * remaining < g_need)
        return false;

    /* Mark this state as visited so we don't redo failed work. */
    size_t idx = ((size_t)pos * MAX_N + rem) * (MAX_NS + 1) + sum;
    if (visit_arr[idx] == visit_tag)
        return false;

    for (int d = 0; d <= 9; ++d) {
        int new_sum = sum + d;
        if (new_sum + 9 * (remaining - 1) < g_need)
            continue;

        int new_rem = (rem * 10 + d) % g_n;
        if (dfs(pos + 1, new_rem, new_sum)) {
            g_digits[pos] = (unsigned char)d;
            return true;
        }
    }

    visit_arr[idx] = visit_tag;  /* remember that this state fails */
    return false;
}

seq_t a(idx_t n)
{
    /* Ensure the visited marker array exists. */
    if (!visit_arr) {
        visit_arr = (unsigned char *)malloc(VISIT_TOTAL);
    }

    /* No special handling needed for n == 0 (offset starts at 1). */
    int n_int = (int)n;                  /* n fits in int (<=10000) */
    int ds = sum_digits_idx_t(n);        /* digit sum of n            */
    int need_sum = n_int - ds;           /* sum that must come from suffix */
    if (need_sum <= 0) {                 /* exact digit sum achieved by n itself */
        return (seq_t)n;
    }

    /* Quick elimination: if the required sum exceeds the maximum
     * achievable with the longest suffix allowed, no solution is possible. */
    if (need_sum > 9 * MAX_P) {
        return 0;
    }

    g_n = n_int;
    g_need = need_sum;

    /* Minimum length of suffix needed to achieve required sum */
    int p_min = (need_sum + 8) / 9;
    if (p_min < 0) p_min = 0;

    for (int p = p_min; p <= MAX_P; ++p) {
        g_p = p;
        /* If with this length the requested sum is still too large, skip. */
        if (need_sum > 9 * p) continue;

        /* Each new search must use a new marker. */
        visit_tag = (visit_tag + 1) & 0xFF;
        if (visit_tag == 0) visit_tag = 1;   /* avoid a zero marker */

        if (!dfs(0, 0, 0))
            continue;                       /* no suffix of this length works */

        /* Build the suffix value from its digits */
        seq_t suffix = 0;
        for (int i = 0; i < p; ++i) {
            suffix = suffix * 10 + (seq_t)g_digits[i];
        }

        /* Build the final number: n * 10^p + suffix */
        seq_t result = (seq_t)n;
        for (int i = 0; i < p; ++i) {
            result *= 10;
        }
        result += suffix;

        return result;
    }

    /* No suitable suffix found for any allowed length. */
    return 0;
}
