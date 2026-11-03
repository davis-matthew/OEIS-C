/*
 * A066378  Floor(log(n)) divides n.
 * https://oeis.org/A066378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 966 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066378: numbers n ≥ 3 such that floor(log_e n) divides n.
   The function returns the n‑th term of the sequence (1‑based index). */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/mem.h"

/* Helper: compute the smallest integer > e^(k+1).
   The result fits in uint64_t for all k that can occur in the
   generated range.  If the value would exceed UINT64_MAX,
   UINT64_MAX is returned. */
static inline uint64_t next_threshold(uint64_t k)
{
    /* Use long double for sufficient exponent range. */
    long double val = expl((long double)(k + 1));
    if (val > (long double)UINT64_MAX) return UINT64_MAX;
    return (uint64_t)ceill(val);
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Static state: memoised terms, current counter and thresholds. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t cur       = 3;        /* current integer to test   */
    static uint64_t cur_k     = 1;        /* floor(log_e cur)          */
    static uint64_t next_lim  = 8;        /* smallest integer > e^(cur_k+1) */

    /* Ensure enough stored terms. */
    while (terms_len < (size_t)n) {
        /* When we pass the current threshold, increment floor(log_e cur). */
        if (cur >= next_lim) {
            ++cur_k;
            next_lim = next_threshold(cur_k);
        }

        /* Test the divisibility condition. */
        if ((cur % cur_k) == 0) {
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 32;
                terms = mem_resize(seq_t, terms, terms_cap);
            }
            terms[terms_len++] = (seq_t)cur;
        }
        ++cur;
    }
    return terms[n - 1];
}
