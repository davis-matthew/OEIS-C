/*
 * A165306  a(n) is the smallest number not yet in the sequence such that
 *          concatenation of all terms yields an infinite periodic stream of
 *          digits 1, 2, 3, ..., 8 (repeat from 1).
 * https://oeis.org/A165306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 29, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165306: a(n) is the smallest integer not yet in the sequence
   such that the concatenation of the sequence up to a(n) is a prefix of
   the infinite periodic stream 1,2,3,4,5,6,7,8,1,2,3,... */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *terms      = NULL;   /* generated terms */
static size_t terms_len  = 0;
static size_t terms_cap  = 0;

static seq_t *used       = NULL;   /* set of used numbers */
static size_t used_len   = 0;
static size_t used_cap   = 0;

static size_t total_digits = 0;    /* number of stream digits consumed */

/* Grow terms array to hold at least "needed" elements */
static inline void grow_terms(size_t needed) {
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!terms) abort();
    terms_cap = new_cap;
}

/* Grow used array to hold at least "needed" elements */
static inline void grow_used(size_t needed) {
    if (needed <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    used = (seq_t *)realloc(used, new_cap * sizeof(seq_t));
    if (!used) abort();
    used_cap = new_cap;
}

/* Check whether val has already been used */
static inline bool used_contains(seq_t val) {
    for (size_t i = 0; i < used_len; ++i) {
        if (used[i] == val) return true;
    }
    return false;
}

/* Generate the next term of the sequence */
static void generate_next(void) {
    seq_t candidate = 0;
    size_t len = 0;
    while (true) {
        int digit = (int)((total_digits + len) % 8 + 1);   /* next digit of 12345678 */
        candidate = candidate * 10 + (seq_t)digit;
        ++len;
        if (!used_contains(candidate)) break;              /* smallest unused found */
    }

    /* Append the new term */
    grow_terms(terms_len + 1);
    terms[terms_len++] = candidate;

    grow_used(used_len + 1);
    used[used_len++] = candidate;

    total_digits += len;   /* advance the stream position */
}

/* Ensure we have generated terms up to index n (1‑based) */
static void ensure_terms(idx_t n) {
    if (n == 0) return;
    size_t target = (size_t)n;
    while (terms_len < target) generate_next();
}

/* Required public function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset starts at 1 */
    ensure_terms(n);
    return terms[(size_t)n - 1];
}
