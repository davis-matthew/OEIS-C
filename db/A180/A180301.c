/*
 * A180301  Starting with 1, the next entry is the next higher integer whose
 *          spelling in US English comes lexicographically later.
 * https://oeis.org/A180301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence-based computation of OEIS A180301.
 *  a(1..11) = {1,2,200,201,202,2000,2001,2002,2200,2201,2202}
 *  For n > 11 we work in blocks of this pattern:
 *      block of size m_k = 12·2^k – 1   (k≥0, m_0=11)
 *      pivot index = m_{k-1}+1 = 12·2^{k-1}
 *      pivot value = 2·10^{E_k}
 *  where the exponents satisfy  E_1 = 12,  E_{k+1} = E_k + 24 + 3·(k-1).
 *  Then
 *      if n == pivot index      -> a(n) = pivot value
 *      else (n > pivot index)   -> a(n) = pivot value + a(n – pivot index)
 *
 *  The sequence is generated recursively without storing all terms.
 *  Works for any idx_t n and for all seq_t types supported by the build.
 */

#include "../utils/pow10.h"
#include <stdbool.h>

#define MAX_LEVELS 70

/* Base values for indices 1..11 */
static const seq_t base[11] = {1, 2, 200, 201, 202, 2000, 2001, 2002, 2200, 2201, 2202};

/* Arrays holding block sizes, pivot indices and values for each level */
static idx_t  m[MAX_LEVELS];           /* block size after given level   */
static idx_t  pivot_idx[MAX_LEVELS];   /* pivot index in that block      */
static seq_t  pivot_val[MAX_LEVELS];   /* pivot value  (2·10^exp)       */
static idx_t  exp_arr[MAX_LEVELS];     /* exponent of 10 in pivot value  */
static idx_t  level_max = 0;           /* highest level computed so far  */

/* Ensure that levels up to a target index are pre‑computed */
static void ensure_levels(idx_t target)
{
    if (level_max == 0) {                /* initialise level 0 */
        m[0]          = 11;              /* size of first block   */
        pivot_idx[0]  = 0;               /* not used              */
        exp_arr[0]    = 0;               /* irrelevant            */
        pivot_val[0]  = 0;               /* irrelevant            */
    }

    while (m[level_max] < target && level_max + 1 < MAX_LEVELS) {
        ++level_max;                     /* new level index       */

        m[level_max]          = 2 * m[level_max-1] + 1;
        pivot_idx[level_max]  = m[level_max-1] + 1;

        if (level_max == 1)
            exp_arr[level_max] = 12;                         /* E_1 = 12   */
        else
            exp_arr[level_max] = exp_arr[level_max-1] + 24 + 3 * (level_max-2);

        seq_t pow_val = pow10(exp_arr[level_max]);           /* 10^E_k      */
        pivot_val[level_max] = pow_val + pow_val;            /* 2·10^E_k     */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 11)                /* base case */
        return base[n-1];

    ensure_levels(n);           /* pre‑compute needed blocks */

    seq_t result = 0;           /* accumulating pivot contributions */

    while (n > 11) {
        /* find minimal level l with n ≤ m[l] */
        idx_t l = 0;
        while (l <= level_max && n > m[l]) ++l;   /* n > m[l] guarantees l > 0 */

        if (n == pivot_idx[l])                /* hit a pivot */
            return result + pivot_val[l];

        /* otherwise n > pivot_idx[l]   */
        result += pivot_val[l];               /* add current pivot value   */
        n -= pivot_idx[l];                    /* move to sub‑index          */
    }

    return result + base[n-1];                 /* add final small part        */
}
