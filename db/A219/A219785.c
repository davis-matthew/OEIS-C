/*
 * A219785  Primes not neighboring an 11-smooth number.
 * https://oeis.org/A219785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   build_primes_upto, is_smooth_11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: primes not neighboring an 11‑smooth number
 * Computed by iterating over primes up to a growing bound, testing for each
 * prime whether p-1 or p+1 is divisible only by 2,3,5,7 and 11.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_smooth_11.h"

static seq_t *seq = NULL;          /* array of found primes */
static size_t seq_len = 0;         /* number of primes stored */
static idx_t sieve_limit = 0;      /* current upper bound of sieve */

/* Rebuild the prime sieve and the sequence up to 'limit' (inclusive). */
static void rebuild_sequence (idx_t limit) {
    uint64_t *p_arr = NULL;
    size_t p_cnt = 0;
    build_primes_upto(limit, &p_arr, &p_cnt);   /* primes <= limit */

    /* free previous data if any */
    if (seq) free(seq);
    seq_len = 0;
    seq = (seq_t*)malloc(16384 * sizeof(seq_t));
    size_t seq_cap = 16384;

    for (size_t i = 0; i < p_cnt; ++i) {
        seq_t p = p_arr[i];
        if (!is_smooth_11(p-1) && !is_smooth_11(p+1)) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = p;
        }
    }
    free(p_arr);
    sieve_limit = limit;
}

/* Ensure the sequence array contains at least 'need' terms. */
static void ensure_terms (idx_t need) {
    if (seq_len >= (size_t)need)
        return;

    /* Start with a reasonable bound and double until enough terms found. */
    idx_t limit = (sieve_limit == 0 ? 2500000 : sieve_limit);
    if (limit < 2500000) limit = 2500000;

    while (seq_len < (size_t)need) {
        rebuild_sequence(limit);
        /* If still insufficient, expand the limit. */
        if (seq_len < (size_t)need)
            limit <<= 1;          /* double the limit */
    }
}

/* The required function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    ensure_terms(n);
    return seq[n-1];
}
