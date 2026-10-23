/*
 * A175837  (2n-1)-abundant numbers.
 * https://oeis.org/A175837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: A175837 – (2n‑1)-abundant numbers.
    This implementation uses a sieve to compute σ(k) and τ(k) up to an
    increasing limit until at least n terms have been found.  No
    hard‑coded values are used.  The implementation is valid C11 and
    C++17 alike. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/*  static state holding already computed terms  */
static seq_t *terms = NULL;
static size_t terms_len = 0;      /* number of terms currently stored   */
static idx_t  current_limit = 0;  /* sieve limit used for the last run  */

/*  Rebuild the sequence until it contains at least target_n terms  */
static void
compute_until(size_t target_n)
{
    /*  Keep expanding the sieve limit until enough terms exist  */
    while (terms_len < target_n) {
        idx_t limit = (current_limit == 0) ? 1000U : current_limit * 2U;

        /*  Allocate arrays for σ(k) and τ(k)  */
        unsigned long long *sigma = (unsigned long long *)calloc((size_t)limit + 1,
                                                                 sizeof(unsigned long long));
        size_t *tau = (size_t *)calloc((size_t)limit + 1, sizeof(size_t));
        if (!sigma || !tau) abort();

        /*  Sieve: for each divisor d add d to σ(m) and increment τ(m)  */
        for (idx_t d = 1; d <= limit; ++d) {
            for (idx_t m = d; m <= limit; m += d) {
                sigma[m] += (unsigned long long)d;
                tau[m]   += 1;
            }
        }

        /*  Build a new list of terms found with the current limit  */
        seq_t *new_terms = NULL;
        size_t new_cap   = 0;
        size_t new_len   = 0;

        for (idx_t k = 1; k <= limit; ++k) {
            /*  Condition: 2*σ(k) > 4*k-2 + τ(k)  */
            unsigned long long left  = 2ULL * sigma[k];
            unsigned long long right = 4ULL * (unsigned long long)k - 2ULL + (unsigned long long)tau[k];
            if (left > right) {
                /*  Append k to the new list  */
                if (new_len == new_cap) {
                    size_t new_cap_val = (new_cap == 0) ? 16U : new_cap * 2U;
                    seq_t *tmp = (seq_t *)realloc(new_terms, new_cap_val * sizeof(seq_t));
                    if (!tmp) abort();
                    new_terms = tmp;
                    new_cap   = new_cap_val;
                }
                new_terms[new_len++] = (seq_t)k;
            }
        }

        /*  Release old storage and adopt the new list  */
        free(sigma);
        free(tau);
        free(terms);
        terms = new_terms;
        terms_len = new_len;
        current_limit = limit;
    }
}

/*  Return the n‑th (1‑based) term of the sequence, for n >= 1   */
seq_t a(idx_t n)
{
    /*  Undefined behaviour for n < 1 – callers must supply n >= 1  */
    compute_until((size_t)n);
    return terms[n - 1];
}
