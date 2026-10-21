/*
 * A230319  Least positive k such that k! > k^n.
 * https://oeis.org/A230319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive k such that k! > k^n, with a(0)=2
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>

static double *log_fact = NULL;  /* ln(k!) values, indexed by k */
static idx_t  log_fact_last = 1; /* highest k with a computed ln(k!) */
static size_t log_fact_size = 0; /* allocated size of log_fact array */

/* Ensure that log_fact[k] is available for all k <= idx.  The array is
   expanded only if necessary and logits are computed incrementally. */
static void ensure_log_fact_up_to(idx_t idx)
{
    if (idx <= log_fact_last)
        return; /* already computed */

    /* Expand allocation if needed */
    if (log_fact_size <= (size_t)idx) {
        size_t new_size = log_fact_size ? log_fact_size * 2 : 2;
        while (new_size <= (size_t)idx)
            new_size <<= 1;
        double *tmp = (double *)realloc(log_fact, new_size * sizeof(double));
        if (!tmp) {
            abort(); /* allocation failure */
        }
        log_fact = tmp;
        log_fact_size = new_size;
    }

    /* Initialize base values if this is the first allocation */
    if (log_fact_last < 1) {
        log_fact[0] = 0.0;
        log_fact[1] = 0.0;
        log_fact_last = 1;
    }

    /* Compute missing entries */
    idx_t start = log_fact_last + 1;
    for (idx_t i = start; i <= idx; ++i) {
        log_fact[(size_t)i] = log_fact[(size_t)(i - 1)] + log((double)i);
    }
    log_fact_last = idx;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;          /* a(0) = 2 by definition */

    idx_t k = 2;                  /* minimal possible k for n>=1 */
    while (1) {
        ensure_log_fact_up_to(k);
        double lhs = log_fact[(size_t)k];                 /* ln(k!)   */
        double rhs = (double)n * log((double)k);          /* n*ln(k)  */
        if (lhs > rhs)
            return (seq_t)k;
        ++k; /* next candidate */
    }
}
