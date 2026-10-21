/*
 * A211560  Number of nonnegative integer arrays of length n+6 with new values
 *          0 upwards introduced in order, and containing the value 6.
 * https://oeis.org/A211560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum{j=7..n+6} Stirling2(n+6,j)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   binom_coef
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom_coef.h"

/* Maximum n that is expected (per requirement) */
#define MAX_N 10000U
#define MAX_M (MAX_N + 6U)

/* ---------- State used for memoisation ---------- */
static seq_t Bell_arr[MAX_M + 1] = {0};      /* Bell numbers B[0..MAX_M]   */
static seq_t S_small[7]       = {1, 0, 0, 0, 0, 0, 0}; /* S(m,j) for j=0..6        */
static seq_t cache[MAX_N]     = {0};              /* a-values for n=1..MAX_N   */

static idx_t current_m = 0;  /* largest m for which Bell_arr[m] is known */

/* Expand internal tables so that values up to m = target_m are known */
static void expand_to_m(idx_t target_m)
{
    if (current_m >= target_m)
        return;

    /* Ensure Bell_arr[0] is set */
    Bell_arr[0] = (seq_t)1;
    /* Ensure current_m is at least 0 */
    if (current_m == 0) {
        /* S_small is already initialized for m = 0 (S(0,0)=1) */
    } else {
        /* For m = current_m + 1 we already have correct S_small */
    }

    for (idx_t m = current_m + 1; m <= target_m; ++m) {
        /* ---- Update S_small for this m ---- */
        /* For j=6 downto 1: S(m,j) = j*S(m-1,j)+S(m-1,j-1) */
        for (idx_t j = 6; j >= 1; --j) {
            S_small[j] = (seq_t)j * S_small[j] + S_small[j - 1];
        }
        S_small[0] = (seq_t)0;  /* S(m,0)=0 for m>0 */

        /* ---- Sum of S_small[j] for j=0..6 ---- */
        seq_t sum_small = 0;
        for (idx_t j = 0; j <= 6; ++j)
            sum_small += S_small[j];

        /* ---- Compute Bell number B_m via recurrence ---- */
        /* B_m = sum_{k=0}^{m-1} C(m-1, k) * B_k */
        idx_t n = m - 1;
        seq_t new_bell = 0;
        for (idx_t k = 0; k <= n; ++k) {
            seq_t binom = binom_coef(n, k);
            new_bell += binom * Bell_arr[k];
        }
        Bell_arr[m] = new_bell;

        /* ---- Store a(n) if m >= 7 ---- */
        if (m >= 7) {
            idx_t idx = m - 6;            /* n = m-6          */
            if (idx <= MAX_N)             /* guard against overflow */
                cache[idx - 1] = new_bell - sum_small;
        }
    }

    current_m = target_m;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* Undefined for n < 1, safe fallback */

    if (n > MAX_N) {
        /* If n exceeds preallocated maximum, extend the tables safely. */
        idx_t new_max = n;
        /* Reallocate static arrays is not possible; call abort to signal error. */
        abort();
    }

    idx_t m = n + 6;                /* We need Bell number B_{n+6} */
    expand_to_m(m);
    return cache[n - 1];
}
