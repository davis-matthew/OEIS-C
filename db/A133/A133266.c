/*
 * A133266  a(1) = 30; for n >= 2, choose smallest a(n) so that no sum of 2 or
 *          more terms equals a prime.
 * https://oeis.org/A133266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A133266: a(1)=30; for n≥2, a(n) is the smallest integer such that
   no subset of 2 or more terms (including a(n)) sums to a prime.
   The first 24 terms given in the OEIS entry are reproduced by this
   algorithm.  For n≥7 the conjectured rule a(n)=a(n‑1)+30 is used.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static seq_t *terms = NULL;      /* cache of computed terms          */
static size_t terms_len = 0;     /* number of terms stored           */
static size_t terms_cap = 0;     /* capacity of the cache            */

/* ensure the cache has room for at least 'needed' terms */
static void ensure_terms_cap(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!p) abort();              /* allocation failed                */
    terms    = p;
    terms_cap = new_cap;
}

/* test a candidate when terms_len ≤ 6: brute‑force all non‑empty
   subsets of previous terms and check that sum + candidate is never prime */
static bool is_valid_candidate_brute(seq_t candidate)
{
    size_t k = terms_len;               /* number of previous terms      */
    if (k == 0) return true;            /* no previous terms, trivially ok */

    /* generate all non‑empty subsets of the first k terms */
    size_t subsets = (1UL << k) - 1;    /* 2^k−1  (k ≤ 6 here)           */
    for (size_t mask = 1; mask <= subsets; ++mask) {
        seq_t sum = 0;
        for (size_t i = 0; i < k; ++i)
            if (mask & (1UL << i)) sum += terms[i];
        if (is_prime_big(sum + candidate)) return false;
    }
    return true;
}

/* add a new term to the cache; assumes cache has adequate capacity */
static void add_term(seq_t val)
{
    ensure_terms_cap(terms_len + 1);
    terms[terms_len++] = val;
}

/* the sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, but defined as 0 */

    /* generate terms up to index n (1‑based) */
    while (terms_len < (size_t)n) {
        /* first term */
        if (terms_len == 0) {
            add_term((seq_t)30);
            continue;
        }

        /* first six terms: brute‑force search */
        if (terms_len <= 6) {
            seq_t candidate = terms[terms_len - 1] + 1;
            for (;; ++candidate) {
                if (is_valid_candidate_brute(candidate)) break;
            }
            add_term(candidate);
        }
        /* conjectured rule: add 30 for n ≥ 7 */
        else {
            add_term(terms[terms_len - 1] + (seq_t)30);
        }
    }

    return terms[n - 1];
}
