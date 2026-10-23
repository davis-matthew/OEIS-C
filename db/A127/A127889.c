/*
 * A127889  Smallest n-digit right-truncatable prime.
 * https://oeis.org/A127889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   A127889 – smallest n‑digit right‑truncatable prime (no simple
 *            closed form; computed by incremental construction)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache for arbitrary order of calls */
    static bool initialized = false;
    static seq_t *seq = NULL;          /* seq[1 .. max] */
    static size_t seq_len = 0;         /* number of valid terms stored */
    static seq_t *cur  = NULL;         /* primes of current length */
    static size_t cur_sz = 0;
    static size_t cur_cap = 0;
    static seq_t *next = NULL;         /* primes of next length */
    static size_t next_sz = 0;
    static size_t next_cap = 0;

    /* illegal indices return 0 */
    if (n < 1) return 0;

    /* one‑time initialisation */
    if (!initialized) {
        unsigned seq_cap = 10001;               /* supports indices up to ~10 000 */
        seq = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        if (!seq) return 0;

        cur_cap = 16;
        cur = (seq_t *)calloc(cur_cap, sizeof(seq_t));
        if (!cur) { free(seq); return 0; }
        cur[0] = 2; cur[1] = 3; cur[2] = 5; cur[3] = 7;
        cur_sz = 4;

        next_cap = 16;
        next = (seq_t *)calloc(next_cap, sizeof(seq_t));
        if (!next) { free(seq); free(cur); return 0; }

        /* the smallest 1‑digit right‑truncatable prime */
        seq[1] = 2;
        seq_len = 1;

        initialized = true;
    }

    /* generate terms until we have at least the requested n */
    for (size_t len = seq_len + 1; len <= (size_t)n; ++len) {
        /* build primes of length 'len' by appending a digit to each prime of length len-1 */
        next_sz = 0;
        for (size_t i = 0; i < cur_sz; ++i) {
            seq_t p = cur[i];
            for (int d = 0; d <= 9; ++d) {
                seq_t v = p * ((seq_t)10) + (seq_t)d;
                if (is_prime_big(v)) {
                    if (next_sz == next_cap) {
                        size_t new_cap = next_cap * 2;
                        seq_t *tmp = (seq_t *)realloc(next, new_cap * sizeof(seq_t));
                        if (!tmp) { free(next); free(cur); free(seq); return 0; }
                        next = tmp;
                        next_cap = new_cap;
                    }
                    next[next_sz++] = v;
                }
            }
        }

        if (next_sz == 0) {
            /* no further right‑truncatable primes exist – fill remaining indices with 0 */
            for (size_t i = len; i <= (size_t)n; ++i) seq[i] = 0;
            return seq[n];
        }

        /* smallest prime of this length becomes a(len) */
        seq_t min_val = next[0];
        for (size_t i = 1; i < next_sz; ++i)
            if (next[i] < min_val) min_val = next[i];
        seq[len] = min_val;
        seq_len = len;

        /* swap cur and next for next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
        cur_sz = next_sz;
        /* cur_cap and next_cap remain adequate for future growth */
    }

    return seq[n];
}
