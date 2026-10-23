/*
 * A131365  Numbers that cannot be written in decimal representation as sum of
 *          distinct repdigits of its digits.
 * https://oeis.org/A131365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1060, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_mask_dp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A131365
    Numbers that cannot be written in decimal representation as sum of distinct
    repdigits of its digits.
    For a number n, let D be the set of distinct decimal digits of n.
    A repdigit is a number consisting of repeated copies of a single digit d
    (e.g. 1, 22, 777).  
    n is representable if there exists a subset S of repdigits each built from a
    digit in D, with all values in S distinct, such that sum_{r in S} r = n.
    This function returns a(n), the nth such non‑representable number.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_mask_dp.h"

static size_t computed_Bmax = 0;          /* Bmax used for current tables       */
static seq_t *stored_terms = NULL;        /* array of a(n) values                 */
static size_t stored_terms_used = 0;      /* number of terms computed             */
static size_t stored_terms_cap = 0;       /* allocated capacity for terms array   */
static uint8_t *mask_dp[1024] = {0};      /* DP bitset for each digit mask        */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* recompute all tables for a new maximum Bmax */
static void recompute_terms(size_t newBmax)
{
    /* free previous tables */
    for (int i = 0; i < 1024; ++i)
    {
        if (mask_dp[i])
        {
            free(mask_dp[i]);
            mask_dp[i] = NULL;
        }
    }
    if (stored_terms)
    {
        free(stored_terms);
        stored_terms = NULL;
    }

    computed_Bmax = newBmax;

    /* init mask_dp array */
    for (int i = 0; i < 1024; ++i)
        mask_dp[i] = NULL;

    /* allocate terms array */
    stored_terms_cap = newBmax;                 /* enough for all non‑representable numbers up to Bmax */
    stored_terms = (seq_t *)malloc(stored_terms_cap * sizeof(seq_t));
    stored_terms_used = 0;

    /* iterate through all numbers up to Bmax and collect non‑representable ones */
    for (size_t num = 1; num <= newBmax; ++num)
    {
        /* build digit mask for num */
        uint16_t mask = 0;
        seq_t t = (seq_t)num;
        while (t > 0)
        {
            int d = (int)(t % 10);
            mask |= (1U << d);
            t /= 10;
        }

        /* compute DP for this mask if not yet computed */
        if (!mask_dp[mask])
            compute_mask_dp(mask, newBmax, &mask_dp[mask]);

        /* check representability */
        if (!mask_dp[mask][num])
        {
            stored_terms[stored_terms_used++] = (seq_t)num;
        }
    }
}

/* ------------------------------------------------------------------ */
/* public function required by the test harness */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                               /* undefined offset */

    /* ensure we have enough terms computed */
    while (stored_terms_used < (size_t)n)
    {
        size_t estimatedB = (size_t)(n * 2 + 100);
        if (estimatedB < 15000) estimatedB = 15000;
        /* if previously computed Bmax is smaller, increase it */
        if (computed_Bmax < estimatedB)
            recompute_terms(estimatedB);
        else
            break;                              /* enough terms already available */
    }

    return stored_terms[n - 1];
}
