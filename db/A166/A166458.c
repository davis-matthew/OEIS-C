/*
 * A166458  Numbers k such that 10*k - (-1)^k is prime.
 * https://oeis.org/A166458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 2803, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, resize_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A166458
   Numbers k such that 10*k - (-1)^k is prime.
   This implementation generates the sequence by testing
   successive integer values of k and storing the ones for which
   the value 10*k - (-1)^k is prime.  The search is memoised, so
   each term is computed only once regardless of the order of
   calls.  The function works for all supported seq_t types
   (fixed‑size unsigned integers or arbitrary‑precision big
   integers). */
 
/*  Include necessary headers for both C11 and C++17.  */
#include <stdlib.h>   /* malloc, realloc, free, size_t */
#include <stdbool.h>  /* bool */
#include <assert.h>   /* assert */
#include <stdint.h>   /* uint64_t */
#include "../utils/is_prime_big.h"   /* is_prime_big for seq_t */
#include "../utils/resize_terms.h"

/* ----------  Memoisation state  ---------- */
static seq_t *terms      = NULL;     /* cached kth terms */
static size_t terms_cap  = 0;        /* current capacity of terms array */
static idx_t  term_cnt   = 0;        /* number of terms already cached */
static idx_t  k_next     = 1;        /* next k value to test */

/* Simple primality test for values that fit into uint64_t. */
static bool is_prime_small(uint64_t n)
{
    if (n < 2u)            return false;
    if (n % 2u == 0u)      return n == 2u;
    for (uint64_t d = 3u; d * d <= n; d += 2u)
        if (n % d == 0u)
            return false;
    return true;
}

/* ----------  Sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;       /* behavior for 0 is irrelevant */

    /* Extend cached sequence until it contains the nth term. */
    while (term_cnt < n)
    {
        /* Compute 10*k - (-1)^k.  For even k: (-1)^k = 1  ... */
        seq_t val;
        if ((k_next & 1u) == 0u)            /* k even */
            val = 10 * (seq_t)k_next - (seq_t)1;
        else                               /* k odd  */
            val = 10 * (seq_t)k_next + (seq_t)1;

        /* Test primality.  Use a fast 64‑bit test when possible. */
        bool prime;
        if (sizeof(seq_t) <= sizeof(uint64_t))
            prime = is_prime_small((uint64_t)val);
        else
            prime = is_prime_big(val);

        /* Store k if it satisfies the condition. */
        if (prime)
        {
            if (term_cnt == terms_cap)
            {
                size_t new_cap = (terms_cap == 0 ? 8 : terms_cap * 2);
                seq_t *tmp = resize_terms(terms, new_cap);
                assert(tmp != NULL);  /* abort if allocation fails */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[term_cnt++] = (seq_t)k_next;
        }

        ++k_next;   /* move on to the next candidate k */
    }

    return terms[n - 1];
}
