/*
 * A037971  Numbers whose maximal base-2 run length is 4.
 * https://oeis.org/A037971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 892 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Numbers whose maximal base-2 run length is 4.
   The sequence is defined as the set of positive integers whose binary
   representation contains a run of either four 0’s or four 1’s and no
   longer run.  The elements are listed in increasing order starting with
   a(1)=15.  
*/
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: test whether the maximum run of equal bits in x is exactly 4. */
static bool max_run_is_4(seq_t x)
{
    int max_run = 0;
    int cur_run = 0;
    int last_bit = -1;   /* -1 means undefined initial bit */

    while (x) {
        int bit = (int)(x & 1);
        if (bit == last_bit) {
            cur_run++;
        } else {
            if (cur_run > max_run) max_run = cur_run;
            cur_run = 1;
            last_bit = bit;
        }
        if (max_run > 4) return false;  /* early exit if we exceed 4 */
        x >>= 1;
    }
    if (cur_run > max_run) max_run = cur_run;
    return max_run == 4;
}

/* Cache of previously found terms.  All static members persist between calls. */
static seq_t *terms          = NULL;   /* array of terms */
static size_t terms_cap      = 0;      /* current allocated capacity */
static size_t terms_count    = 0;      /* number of terms stored */
static uint64_t next_candidate = 1;     /* next integer to test */

static void ensure_terms_capacity(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_terms = mem_resize(seq_t, terms, new_cap);
    if (!new_terms) abort();  /* OOM: terminate */
    terms = new_terms;
    terms_cap = new_cap;
}

/* Main function: return the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of range, undefined behaviour not required */
    /* initialise cache on first use */
    if (!terms) {
        terms_cap = 16;
        terms = mem_new(seq_t, terms_cap);
        if (!terms) abort();
    }

    while (terms_count < (size_t)n) {
        while (1) {
            seq_t cand = (seq_t)next_candidate;
            if (max_run_is_4(cand)) {
                /* store found term */
                ensure_terms_capacity(terms_count + 1);
                terms[terms_count++] = cand;
                break;  /* move to next candidate */
            }
            next_candidate++;
        }
        /* move past the number we just stored */
        next_candidate++;
    }

    return terms[n - 1];
}
