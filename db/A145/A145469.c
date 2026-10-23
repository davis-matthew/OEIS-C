/*
 * A145469  Positive integers k such that d(k) = d(k-1) + d(k-2), where d(k)
 *          is the number of divisors of k.
 * https://oeis.org/A145469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 2762, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq, divisor_count_u64, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Positive integers k such that d(k) = d(k-1) + d(k-2)
   (d(k) is the number of divisors of k). The function scans
   integers, checks the divisor‑count condition, and memoises
   the qualifying k. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/divisor_count_u64.h"
#include "../utils/alloc_seq.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Memoisation state */
    static seq_t *terms      = NULL;   /* stored terms */
    static size_t terms_cap  = 0;      /* capacity of array */
    static size_t terms_len  = 0;      /* number of terms stored */

    static idx_t next_k      = 3;      /* next integer to test   */
    static seq_t d_prev2     = 1;      /* d(1) */
    static seq_t d_prev1     = 2;      /* d(2) */
    static int  initialized  = 0;      /* one‑time initialisation */

    if (!initialized) {
        next_k   = 3;
        d_prev2  = 1;
        d_prev1  = 2;
        initialized = 1;
    }

    /* Build more terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        seq_t d_k = (seq_t)divisor_count_u64((uint64_t)next_k);

        if (d_k == d_prev1 + d_prev2) {
            /* Grow array if necessary */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 4;
                seq_t *tmp = seq_realloc(terms, terms_cap, new_cap);
                if (!tmp) abort();   /* memory exhaustion */
                terms      = tmp;
                terms_cap  = new_cap;
            }
            terms[terms_len++] = (seq_t)next_k;
        }

        d_prev2 = d_prev1;
        d_prev1 = d_k;
        next_k++;
    }

    return terms[n - 1];
}
