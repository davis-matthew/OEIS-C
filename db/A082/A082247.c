/*
 * A082247  Multiples of 7 that are concatenation of 7 consecutive natural
 *          numbers.
 * https://oeis.org/A082247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 13, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates the sequence A082247:
   numbers that are the concatenation of 7 consecutive natural numbers
   and divisible by 7.  No closed‑form formula is known, so we
   compute each term by brute force search.
   The sequence is indexed starting at 1. */

#include <stdint.h>
#include <stddef.h>

/* Helper: number of decimal digits of an unsigned 64‑bit integer. */
static inline uint64_t digit_count_uint64(uint64_t n) {
    if (n == 0) return 1;
    uint64_t d = 0;
    while (n) { n /= 10; ++d; }
    return d;
}

/* Precomputed powers of ten (up to 10^20). */
static seq_t pow10_cache[21];
static int pow10_inited = 0;
static void init_pow10_cache(void) {
    if (pow10_inited) return;
    pow10_cache[0] = 1;
    for (int i = 1; i <= 20; ++i) {
        pow10_cache[i] = pow10_cache[i-1] * 10;
    }
    pow10_inited = 1;
}

/* Concatenate the 7 numbers k .. k+6 and return the integer value. */
static seq_t concat_k(idx_t k) {
    seq_t v = 0;
    for (idx_t t = k; t < k + 7; ++t) {
        uint64_t len = digit_count_uint64((uint64_t)t);
        v = v * pow10_cache[len] + (seq_t)t;
    }
    return v;
}

/* Static cache of found terms.  10000 is enough for the required limits
   and keeps the program deterministic and purely static. */
static seq_t term_cache[10000];
static size_t term_len   = 0;     /* how many terms stored */
static idx_t  last_k     = 0;     /* start of the last found block */

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for n<1 */

    init_pow10_cache();

    /* Compute terms until we reach the requested index. */
    while (term_len < (size_t)n) {
        idx_t k = last_k + 1;            /* start searching from next integer */
        seq_t val;

        /* Find first k such that the concatenation is a multiple of 7. */
        while (1) {
            val = concat_k(k);
            if (val % 7 == 0) break;
            ++k;
        }

        term_cache[term_len++] = val;     /* store the found term */
        last_k = k;                      /* remember start for next search */
    }

    return term_cache[n - 1];
}
