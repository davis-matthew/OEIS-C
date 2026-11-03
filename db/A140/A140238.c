/*
 * A140238  Numbers k such that Sum_{i=1..k} d(i) is coprime to d(k), where
 *          d(k) is the number of positive divisors of k.
 * https://oeis.org/A140238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 3270, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9868 (2026-11-03)
 * Helpers:   gcd, mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A140238:
 *   Numbers k such that the cumulative sum of divisor counts up to k,
 *   ∑_{i=1..k} d(i), is coprime with d(k).
 *
 *  The function returns the n‑th such k (1‑based indexing).
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/num_divisors.h"
#include "../utils/mem.h"

/* Cache of found terms */
static seq_t *cached_terms = NULL;
static size_t cached_len  = 0;    /* number of terms stored */
static size_t cached_cap  = 0;    /* capacity of cached_terms */
static idx_t  processed_k = 0;    /* largest k processed so far */
static seq_t  sum_div     = 0;    /* cumulative divisor sum up to processed_k */

/* Append a new term to the cache, growing the array if needed. */
static void append_term(idx_t k)
{
    if (cached_len == cached_cap) {
        size_t new_cap = (cached_cap == 0) ? 64 : cached_cap * 2;
        /* cast required for C++ compatibility */
        seq_t *tmp = mem_resize(seq_t, cached_terms, new_cap);
        if (!tmp) abort();
        cached_terms = tmp;
        cached_cap   = new_cap;
    }
    cached_terms[cached_len++] = (seq_t)k;
}

/* Return the n-th term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but avoid UB */

    while (cached_len < (size_t)n) {
        ++processed_k;
        uint64_t d_k  = num_divisors((uint64_t)processed_k);
        seq_t dseq   = (seq_t)d_k;
        seq_t new_sum = sum_div + dseq;

        if (gcd(new_sum, dseq) == 1) {
            append_term(processed_k);
        }
        sum_div = new_sum;
    }
    return cached_terms[n - 1];
}
