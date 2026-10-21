/*
 * A082616  Palindromes such that the least common multiple of any pair of
 *          successive terms is a palindrome.
 * https://oeis.org/A082616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   gcd, is_palindrome, seq_alloc, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A082616:
   a(n) = the nth decimal palindrome such that
   LCM(a(n‑1), a(n)) is also a palindrome (a(1)=1). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/is_palindrome.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cache for the computed terms */
static seq_t *cache = NULL;      /* array of accepted palindromes */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* current length of the cache */
static seq_t last_term = 0;      /* last accepted palindrome (if any) */
static idx_t next_candidate = 1;/* next integer to test for palindrome */

/* Append a new term, reallocating if necessary. */
static inline void push_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Least common multiple, computed with integer arithmetic. */
static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    return seq_mul(a / g, b);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; behavior for n<1 is irrelevant */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate; /* candidate palindrome */

        /* Accept only if cand is a palindrome. */
        if (is_palindrome(cand, 10)) {
            if (cache_len == 0) { /* first term, accept automatically */
                push_term(cand);
                last_term = cand;
            } else {
                /* Check palindrome property of the LCM with the last term. */
                seq_t l = lcm_seq(last_term, cand);
                if (is_palindrome(l, 10)) {
                    push_term(cand);
                    last_term = cand;
                }
            }
        }
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
