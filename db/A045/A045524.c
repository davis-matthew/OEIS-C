/*
 * A045524  Numbers k such that k! has initial digit '5'.
 * https://oeis.org/A045524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 5128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045524: Numbers k such that k! starts with the digit 5. */
/* Computation uses a running sum of log10(k) to extract the leading digit
   of each factorial. The n-th term is returned in a static cache. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* static state for incremental construction of the sequence */
    static idx_t last_k = 1;               /* last k processed (1! processed) */
    static double log_sum = 0.0;           /* sum of log10(2..last_k)   */
    static double log10_5 = 0.0, log10_6 = 0.0; /* thresholds for leading 5   */
    static seq_t *term_cache = NULL;       /* cached terms              */
    static size_t term_cap = 0;            /* capacity of cache         */
    static size_t term_size = 0;           /* number of cached terms    */
    static bool initialized = false;       /* one‑time initialization   */

    /* one‑time set up */
    if (!initialized) {
        log10_5 = log10(5.0);
        log10_6 = log10(6.0);
        term_cap = 64;
        term_cache = (seq_t*)malloc(term_cap * sizeof(seq_t));
        term_size = 0;
        initialized = true;
    }

    /* Grow cache until it contains at least n terms */
    while (term_size < n) {
        ++last_k;                          /* next factorial to examine */
        log_sum += log10((double)last_k);  /* accumulate log10(k)       */

        /* extract fractional part of log10(last_k!) */
        double intpart = (double)(long long)log_sum;
        double frac = log_sum - intpart;

        /* leading digit is 5 iff fractional part lies in [log10(5), log10(6)) */
        if (frac >= log10_5 && frac < log10_6) {
            /* store the term */
            if (term_size >= term_cap) {
                term_cap *= 2;
                term_cache = (seq_t*)realloc(term_cache, term_cap * sizeof(seq_t));
            }
            term_cache[term_size++] = (seq_t)last_k;
        }
    }

    return term_cache[n - 1];
}
