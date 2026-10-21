/*
 * A054913  Number of labeled connected graphs with n nodes such that
 *          complement is also connected.
 * https://oeis.org/A054913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A001187(n) - A006125(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binom, pow2, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 2 * (number of connected labelled graphs on n vertices) 
          - 2^(n*(n-1)/2).  */

#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Memoised tables */
static seq_t *C_tbl = NULL;      /* C_tbl[i] = number of connected graphs on i vertices */
static idx_t  C_max = 0;         /* largest n for which C_tbl is computed */

static seq_t *pow2_tbl = NULL;   /* pow2_tbl[i] = 2^(i*(i-1)/2) */
static idx_t  pow2_max = 0;      /* largest i for which pow2_tbl is computed */

/* Ensure pow2_tbl contains 2^(i*(i-1)/2) for 0 <= i <= n */
static void ensure_pow2_tbl(idx_t n)
{
    if (n <= pow2_max) return;

    seq_t *old = pow2_tbl;
    pow2_tbl = seq_alloc_new_array(n + 1);
    if (!pow2_tbl) exit(1);

    /* copy previous values if any */
    if (old) {
        for (idx_t i = 0; i <= pow2_max; ++i)
            pow2_tbl[i] = old[i];
        seq_free_array(old);
    }

    for (idx_t i = pow2_max + 1; i <= n; ++i) {
        idx_t exp = (i * (i - 1)) / 2;
        pow2_tbl[i] = pow2(exp);          /* helper returning 2^exp as seq_t */
    }
    pow2_max = n;
}

/* Ensure C_tbl contains 2*A001187 values up to n */
static void ensure_connected(idx_t n)
{
    if (n <= C_max) return;

    if (C_tbl == NULL) {
        C_tbl = seq_alloc_new_array(1);   /* index 0 unused, but allocate to avoid NULL */
        if (!C_tbl) exit(1);
    }

    /* allocate new space if needed */
    seq_t *old = C_tbl;
    C_tbl = seq_alloc_new_array(n + 1);
    if (!C_tbl) exit(1);
    if (old) {
        for (idx_t i = 0; i <= C_max; ++i)
            C_tbl[i] = old[i];
        seq_free_array(old);
    }

    /* initialise new entries to 0 (C_tbl[1] will be set below) */
    for (idx_t i = C_max + 1; i <= n; ++i)
        C_tbl[i] = 0;
    if (C_max < 1)
        C_tbl[1] = 1;     /* base value: one connected graph on a single vertex */

    ensure_pow2_tbl(n);

    for (idx_t m = (C_max > 1 ? C_max + 1 : 2); m <= n; ++m) {
        seq_t total = pow2_tbl[m];
        seq_t sum   = 0;
        seq_t binom = 1;              /* binom(m-1, 0) */

        for (idx_t k = 1; k <= m - 1; ++k) {
            seq_t rest_pow2 = pow2_tbl[m - k];
            seq_t term = binom * C_tbl[k];
            term = term * rest_pow2;
            sum = sum + term;

            /* update binom to binom(m-1, k) */
            binom = binom * (seq_t)(m - k);
            binom = binom / k;
        }
        C_tbl[m] = total - sum;
    }
    C_max = n;
}

/* Required OEIS sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1 */

    ensure_connected(n);
    ensure_pow2_tbl(n);

    seq_t result = C_tbl[n];
    result = result + result;     /* multiply by 2 */
    result = result - pow2_tbl[n];
    return result;
}
