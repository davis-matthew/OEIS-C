/*
 * A057490  Numbers k that divide 8^k + 7^k + 6^k + 5^k + 4^k + 3^k + 2^k.
 * https://oeis.org/A057490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Condition: k divides 8^k+7^k+6^k+5^k+4^k+3^k+2^k
   The function returns the n-th positive integer k with that property.
   It searches sequentially and memoizes discovered terms so that repeated
   unordered queries are efficient. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

static seq_t *terms = NULL;       /* array of found terms */
static size_t term_count = 0;     /* number of terms stored */
static size_t term_cap = 0;       /* allocated capacity */
static idx_t  checked_upto = 0;   /* last k checked */

/* Grow the terms array if needed. */
static void ensure_capacity(size_t needed)
{
    if (needed <= term_cap) return;
    size_t new_cap = term_cap ? term_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) {
        /* In an embedded environment, aborting is acceptable. */
        abort();
    }
    terms = new_terms;
    term_cap = new_cap;
}

/* Test whether k satisfies the divisibility condition. */
static bool is_good_k(idx_t k)
{
    seq_t kseq = (seq_t)k;
    seq_t sum = 0;
    /* Sum i^k for i=2..8 (mod k). */
    for (int base = 2; base <= 8; ++base) {
        seq_t p = mod_pow((seq_t)base, k, kseq);
        sum += p;
    }
    return (sum % kseq) == 0;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    /* If we already have the term, return it. */
    if ((size_t)n <= term_count) {
        return terms[(size_t)n - 1];
    }

    /* Search for additional terms until we reach the desired index. */
    for (idx_t k = checked_upto + 1; ; ++k) {
        if (is_good_k(k)) {
            /* Store the term. */
            ensure_capacity(term_count + 1);
            terms[term_count++] = (seq_t)k;
            if ((size_t)n <= term_count) {
                /* Desired term found. */
                checked_upto = k;
                return terms[(size_t)n - 1];
            }
        }
    }
}
