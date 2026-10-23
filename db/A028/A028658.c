/*
 * A028658  Expansion of (theta_3(z)*theta_3(23z) +
 *          theta_2(z)*theta_2(23z))^2.
 * https://oeis.org/A028658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, floor_sqrt, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A028658: Expansion of (theta_3(z)*theta_3(23z) + theta_2(z)*theta_2(23z))^2.
 * This function computes the n‑th term a(n) for any n ≥ 0.
 * It builds the coefficient array for the inner sum once and reuses it
 * thereafter.  No external libraries beyond those listed are required.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_realloc.h"
#include "../utils/floor_sqrt.h"

/* static storage for the combined coefficient array e[k] = c[k] + d[k] */
static seq_t *e = NULL;
static size_t e_cap = 0;

/* helper: compute e[n] (the coefficient of the inner sum at exponent n) */
static seq_t compute_e(idx_t n)
{
    seq_t res = 0;

    /* --- theta_3(z) * theta_3(23z) part --- */
    seq_t nn = (seq_t)n;
    seq_t maxk = floor_sqrt(nn);
    for (seq_t k = 0; k <= maxk; ++k)
    {
        seq_t k2    = k * k;
        seq_t rem   = nn - k2;
        if (rem % 23 != 0) continue;
        seq_t l2    = rem / 23;
        seq_t l     = floor_sqrt(l2);
        if (l * l != l2) continue;
        unsigned multK = (k == 0) ? 1 : 2;
        unsigned multL = (l == 0) ? 1 : 2;
        res += (seq_t)(multK * multL);
    }

    /* --- theta_2(z) * theta_2(23z) part --- */
    /* Write 4*n = (2k+1)^2 + 23(2l+1)^2  with odd integers (2k+1),(2l+1) */
    seq_t four_n = 4 * nn;
    seq_t maxm   = floor_sqrt(four_n);
    for (seq_t m = 1; m <= maxm; m += 2)   /* m odd */
    {
        seq_t m2 = m * m;
        if (m2 > four_n) break;
        seq_t rem = four_n - m2;
        if (rem % 23 != 0) continue;
        seq_t l2 = rem / 23;
        seq_t l  = floor_sqrt(l2);
        if (l * l != l2) continue;
        if (l % 2 != 1) continue;          /* l must be odd */
        res += 4;   /* all ± combinations give multiplicity 4 */
    }

    return res;
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    /* Ensure the coefficient array e is large enough */
    if ((idx_t)e_cap <= n)
    {
        seq_t *new_e = seq_realloc(e, e_cap, (size_t)n + 1);
        if (new_e == NULL)
        {
            /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        e = new_e;
        /* compute new elements */
        for (idx_t i = e_cap; i <= n; ++i)
        {
            e[i] = compute_e(i);
        }
        e_cap = (size_t)n + 1;
    }

    /* Compute the convolution a[n] = sum_{k=0}^n e[k]*e[n-k] */
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        result += e[k] * e[n - k];
    }
    return result;
}
