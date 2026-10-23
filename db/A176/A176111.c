/*
 * A176111  Honaker primes of the form p = 2*k-1 with sum-of-digits(p) =
 *          sum-of-digits(k).
 * https://oeis.org/A176111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 28, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   The function enumerates Honaker primes that also satisfy the extra
   condition p = 2*k-1 with digit_sum(p) = digit_sum(k).  Let p be a prime,
   let j be its index in the ordered list of primes (so p = prime(j)),
   and let k = (p+1)/2.  The sequence term is p when
   digit_sum(p) == digit_sum(j) == digit_sum(k).

   The implementation expands the sequence incrementally, caching found
   terms so that subsequent calls with any index are answered in
   amortised O(1) time after the needed terms have been generated.
*/

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_big.h"

/* cache of previously found terms */
static seq_t *term_cache = NULL;
static idx_t term_len   = 0;
static idx_t term_cap   = 0;

/* state for incremental prime generation */
static idx_t prime_index   = 0;    /* number of primes seen so far */
static idx_t candidate    = 2;    /* next integer to test for primality */

/* append a new term to the cache */
static void append_term(seq_t v)
{
    if (term_len == term_cap) {
        term_cap = term_cap ? term_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(term_cache, term_cap * sizeof(seq_t));
        if (!tmp) abort();
        term_cache = tmp;
    }
    term_cache[term_len++] = v;
}

/* Return the nth term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n = 0 */

    /* Generate terms until we have at least n of them */
    while (term_len < n) {
        seq_t p = (seq_t)candidate;

        if (is_prime_big(p)) {
            prime_index++;                 /* p is the prime_index‑th prime */
            idx_t j = prime_index;         /* index j of p */

            /* compute k = (p+1)/2 */
            seq_t k = (p + (seq_t)1) / (seq_t)2;

            /* digit sums */
            seq_t ds_p = digit_sum(p);
            seq_t ds_j = digit_sum((seq_t)j);
            seq_t ds_k = digit_sum(k);

            if (ds_p == ds_j && ds_p == ds_k) {
                append_term(p);
            }
        }

        /* advance candidate */
        if (candidate == 2) {
            candidate = 3;   /* next odd number after 2 */
        } else {
            candidate += 2;   /* skip even numbers */
        }
    }

    return term_cache[n - 1];
}
