/*
 * A066191  Numbers k such that the sum of the odd aliquot parts of k divides
 *          k.
 * https://oeis.org/A066191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 6654, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066191:
   Numbers k such that the sum of the odd aliquot parts of k divides k. */

#include <stdlib.h>
#include <stdint.h>

static size_t terms_count = 0;
static size_t terms_cap   = 0;
static seq_t *terms = NULL;

static size_t sigma_cap   = 0;
static uint64_t *sigma_all = NULL;
static uint64_t *odd_sum   = NULL;

/* Rebuild the divisor sum tables up to 'limit'. */
static void recompute_tables(size_t limit)
{
    /* Allocate arrays for sigma_all and odd_sum. */
    sigma_all = (uint64_t*)realloc(sigma_all, (limit + 1) * sizeof(uint64_t));
    odd_sum   = (uint64_t*)realloc(odd_sum,   (limit + 1) * sizeof(uint64_t));

    for (size_t i = 0; i <= limit; ++i) {
        sigma_all[i] = 0;
        odd_sum[i]   = 0;
    }

    /* Classic divisor sum sieve. */
    for (size_t d = 1; d <= limit; ++d) {
        for (size_t m = d; m <= limit; m += d) {
            sigma_all[m] += d;
            if (d & 1)        /* odd divisor */
                odd_sum[m]   += d;
        }
    }
    sigma_cap = limit;
}

/* Rebuild the sequence terms up to 'limit'. */
static void rebuild_terms(size_t limit)
{
    recompute_tables(limit);

    /* Allocate space for the terms. The number of terms is at most 'limit'. */
    seq_t *new_terms = (seq_t*)realloc(terms, (limit + 1) * sizeof(seq_t));
    terms_cap        = limit + 1;
    terms            = new_terms;

    size_t pos = 0;
    for (size_t k = 1; k <= limit; ++k) {
        uint64_t sum;
        if ((k & 1) == 0) {          /* even k: sum of all odd divisors */
            sum = odd_sum[k];
        } else {                     /* odd k: sum of odd proper divisors */
            sum = sigma_all[k] - k;
        }
        if (sum != 0 && (k % sum) == 0) {
            terms[pos++] = (seq_t)k;
        }
    }
    terms_count = pos;
}

/* Ensure that the array contains at least 'n' terms. */
static void ensure_n_terms(idx_t n)
{
    while ((size_t)n > terms_count) {
        size_t new_limit = (terms_cap == 0) ? 1
                                            : (size_t)terms_cap * 2;
        rebuild_terms(new_limit);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; return 0 for safety */

    ensure_n_terms(n);
    return terms[(size_t)n - 1];
}
