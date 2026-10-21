/*
 * A204823  Sum of divisors (A000203) of deficient numbers (A005100).
 * https://oeis.org/A204823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = σ( dₙ ) where dₙ is the n‑th deficient number (A005100)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes the sum‑of‑divisors σ up to a
   bound that is guaranteed to contain the n‑th deficient number
   for n ≤ 10000.  The bound is 5 × n + 100.  If it fails for
   unusually large n it keeps doubling until the n‑th deficient
   number is found. */

#include <stdlib.h>

static seq_t *sigma_arr = NULL;   /* array of σ values */
static size_t sigma_cap = 0;      /* allocated size of sigma_arr */

seq_t a(idx_t n)
{
    /* Compute an upper bound that surely contains the n‑th deficient number. */
    idx_t limit = n * 5 + 100;
    size_t new_cap = (size_t)limit + 1;   /* inclusive of index 0 */

    /* Allocate or resize the σ array when needed. */
    if (new_cap > sigma_cap) {
        free(sigma_arr);
        sigma_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        sigma_cap = new_cap;

        /* initialise array */
        for (size_t i = 0; i < sigma_cap; ++i) sigma_arr[i] = 0;

        /* compute σ(i) for all 1 ≤ i ≤ limit */
        for (idx_t i = 1; i <= limit; ++i) {
            for (idx_t j = i; j <= limit; j += i) {
                sigma_arr[j] += (seq_t)i;
            }
        }
    }

    /* scan for the n‑th deficient number */
    seq_t result = 0;
    idx_t count  = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (sigma_arr[i] < ((seq_t)2 * (seq_t)i)) {
            ++count;
            if (count == n) {
                result = sigma_arr[i];
                break;
            }
        }
    }

    /* In the unlikely event the bound was insufficient, double it
       and recompute until the n‑th term is found. */
    while (count < n) {
        limit = limit * 2 + 100;
        new_cap = (size_t)limit + 1;
        free(sigma_arr);
        sigma_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        sigma_cap = new_cap;

        for (size_t i = 0; i < sigma_cap; ++i) sigma_arr[i] = 0;
        for (idx_t i = 1; i <= limit; ++i) {
            for (idx_t j = i; j <= limit; j += i) {
                sigma_arr[j] += (seq_t)i;
            }
        }

        count = 0;
        for (idx_t i = 1; i <= limit; ++i) {
            if (sigma_arr[i] < ((seq_t)2 * (seq_t)i)) {
                ++count;
                if (count == n) {
                    result = sigma_arr[i];
                    break;
                }
            }
        }
    }

    return result;
}
