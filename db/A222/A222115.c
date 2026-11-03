/*
 * A222115  a(n) = 1 + Sum_{k=1..n} binomial(n,k) * sigma(k).
 * https://oeis.org/A222115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n} binomial(n,k) * sigma(k)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 936 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (a(1) is the first term)                                     */

#include <stdlib.h>   /* malloc, free */
#include "../utils/mem.h"

/* Expand the table of divisor sums (sigma) up to at least `n`.  The
   table is stored in a static array that grows only when needed.       */
static void _ensure_sigma(idx_t n, seq_t **sigma_ptr, idx_t *size_ptr)
{
    if (*size_ptr && n + 1 <= *size_ptr)
        return;                                       /* already large enough */

    /* Allocate a new table, zero‑initialised.  Cast the result of
       malloc to the proper type for C++ as well as C.                */
    seq_t *new_sigma = mem_new(seq_t, (n + 1));
    if (!new_sigma)
        abort();                                      /* out of memory */

    for (size_t i = 0; i <= (size_t)n; ++i)
        new_sigma[i] = 0;

    /* Compute sigma values for all m ≤ n  by the divisor–sieve method. */
    for (idx_t d = 1; d <= n; ++d)
    {
        seq_t d_seq = (seq_t)d;                       /* avoid repeated cast */
        for (idx_t m = d; m <= n; m += d)
            new_sigma[m] += d_seq;
    }

    /* Replace the old table.  free(NULL) is safe.                 */
    mem_free(*sigma_ptr);
    *sigma_ptr  = new_sigma;
    *size_ptr   = n + 1;
}

/* Public sequence function.  Computes a(n) for any n ≥ 1.         */
seq_t a(idx_t n)
{
    static seq_t *sigma = NULL;
    static idx_t sigma_size = 0;

    /* Grow the sigma table if needed. */
    _ensure_sigma(n, &sigma, &sigma_size);

    seq_t result = 1;      /* the constant "+1" in the definition      */
    seq_t binom  = 1;      /* C(n,0)                                 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update binomial coefficient multiplicatively.  All
           intermediate operations stay integer because (n-k+1) is
           divisible by k in the division.                 */
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;
        result += binom * sigma[k];
    }

    return result;
}
