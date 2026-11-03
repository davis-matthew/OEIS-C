/*
 * A062201  Number of compositions of n such that two adjacent parts are not
 *          equal modulo 3.
 * https://oeis.org/A062201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n such that no two adjacent
 *            parts are congruent modulo 3. The function uses a simple
 *            dynamic‑programming recurrence with memoisation
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 37 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n >= 0; a(0) = 1 (empty composition).  */
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static memoisation state: only updated when a larger n is requested  */
    static idx_t max_n = 0;               /* maximum n already computed */
    static seq_t *total = NULL;           /* total[i] = a(i)                */
    static seq_t *f[3] = { NULL, NULL, NULL }; /* f[r][i] = #compositions of i
                                               ending with part ≡ r (mod 3) */

    /* If we have never allocated the tables, do so for n == 0.  */
    if (total == NULL)
    {
        /* newcap = 1 for n == 0; for larger n allocation will be done later */
        idx_t newcap = (n > 0) ? (n + 1) : 1;
        total = mem_new(seq_t, newcap);
        f[0]   = mem_new(seq_t, newcap);
        f[1]   = mem_new(seq_t, newcap);
        f[2]   = mem_new(seq_t, newcap);

        /* initialise the base case: a(0) = 1; no ending residue for empty comp. */
        total[0] = (seq_t)1;
        f[0][0] = f[1][0] = f[2][0] = (seq_t)0;
        max_n = 0;

        /* If the caller asked for n==0, we are done. */
        if (n == 0)
            return total[0];
    }

    /* Grow tables if a larger n is requested.  */
    if (n > max_n)
    {
        idx_t newcap = n + 1;          /* indices 0 .. n inclusive */
        total = mem_resize(seq_t, total, newcap);
        f[0]   = mem_resize(seq_t, f[0], newcap);
        f[1]   = mem_resize(seq_t, f[1], newcap);
        f[2]   = mem_resize(seq_t, f[2], newcap);

        /* Compute values from max_n+1 up to n.  */
        for (idx_t i = max_n + 1; i <= n; ++i)
        {
            for (int r = 0; r < 3; ++r)
            {
                seq_t val = (seq_t)0;
                /* first part k with k%3 == r (positive).  */
                idx_t k_start = (r == 0) ? 3 : (idx_t)r;

                for (idx_t k = k_start; k <= i; k += 3)
                {
                    if (k == i)          /* single part composition */
                        val += (seq_t)1;
                    else
                    {
                        idx_t j = i - k;             /* rest of the sum */
                        /* compositions of j whose last part ≠ r (mod 3) */
                        val += total[j] - f[r][j];
                    }
                }
                f[r][i] = val;
            }
            total[i] = f[0][i] + f[1][i] + f[2][i];
        }
        max_n = n;
    }

    return total[n];
}
