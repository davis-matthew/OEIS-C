/*
 * A182226  Abundant numbers m (in A005101) whose sum of divisors less than
 *          their abundance A033880(m) does not exceed A033880(m).
 * https://oeis.org/A182226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 2664, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, sum_div_lower
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A182226: Abundant numbers m whose sum of divisors less than their abundance
   A033880(m) does not exceed the abundance.  The function scans integers,
   tests the condition, and caches the resulting terms for efficient repeated
   calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/sum_div_lower.h"

/* Main sequence generator. */
seq_t a(idx_t n)
{
    static idx_t candidate = 2;          /* integer to test (start from 2) */
    static size_t terms_cap = 0;        /* capacity of terms array */
    static size_t terms_len  = 0;        /* number of cached terms */
    static seq_t *terms = NULL;         /* cached sequence terms */

    if (n == 0) return 0;               /* undefined for n < 1, safe fallback */

    size_t needed = (size_t)n;
    while (terms_len < needed) {
        uint64_t m = (uint64_t)candidate;
        uint64_t pr[64], ex[64];
        size_t cnt = 0;
        factor_u64(m, pr, ex, &cnt);

        /* Compute sigma(m) = product over primes of (p^(e+1)-1)/(p-1). */
        uint64_t sigma = 1;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = pr[i];
            uint64_t e = ex[i];
            uint64_t powpe = 1;
            for (uint64_t k = 0; k <= e; ++k) {
                powpe *= p;
            }
            uint64_t term = (powpe - 1) / (p - 1);
            sigma *= term;
        }

        uint64_t a_val = sigma > 2*m ? sigma - 2*m : 0;
        if (a_val > 0) {
            uint64_t sumLow = 0;
            bool stop = false;
            sum_div_lower(pr, ex, cnt, 0, 1, a_val, &sumLow, &stop);
            if (!stop && sumLow <= a_val) {
                /* Store the term. */
                if (terms_len + 1 > terms_cap) {
                    size_t newcap = terms_cap == 0 ? 16 : terms_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                    if (!tmp) { /* allocation failed – abort silently */ }
                    terms = tmp;
                    terms_cap = newcap;
                }
                terms[terms_len++] = (seq_t)m;
            }
        }
        ++candidate;
    }
    return terms[needed - 1];
}
