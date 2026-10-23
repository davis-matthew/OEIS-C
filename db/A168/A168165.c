/*
 * A168165  Primes which are the concatenation of a Sophie Germain prime p and
 *          2p+1.
 * https://oeis.org/A168165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 3, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   concat_parts, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by searching for Sophie–Germain primes p and their
   safe‑prime partner q = 2p+1, concatenating p and q, testing the
   concatenated number for primality, and caching the found terms. */

#include <stdlib.h>   /* abort, malloc, realloc */
#include <stdint.h>   /* uint64_t */

/* Utility headers that provide the required functions. */
#include "../utils/is_prime.h"       /* bool is_prime(idx_t x); */
#include "../utils/concat_parts.h"   /* seq_t concat_parts(idx_t a, idx_t b); */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

/* Static cache of computed terms. */
static seq_t  *terms      = NULL;  /* array of found terms           */
static size_t terms_cap   = 0;     /* current capacity of 'terms'    */
static idx_t  terms_len   = 0;     /* number of terms currently stored */
static idx_t  last_p_chk  = 1;     /* last prime examined (start at 1) */

/* Ensure the terms array can hold at least 'need' elements. */
static void
ensure_terms_capacity(size_t need)
{
    if (need <= terms_cap)
        return;

    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                       /* allocation failure */
    terms      = tmp;
    terms_cap  = new_cap;
}

/* Main sequence function: returns the n-th term (n>=1). */
seq_t
a(idx_t n)
{
    if (n == 0)                      /* offset is 1; undefined for 0 */
        return (seq_t)0;

    /* Return cached term if already available. */
    if (n <= terms_len)
        return terms[n - 1];

    /* Keep searching until we have at least n terms. */
    while (terms_len < n) {
        /* Find the next Sophie–Germain prime p. */
        for (idx_t p = last_p_chk; ; ++p) {
            if (!is_prime(p))
                continue;           /* p must be prime */

            idx_t q = 2 * p + 1;      /* safe prime candidate */
            if (!is_prime(q))
                continue;           /* q must be prime */

            /* Concatenate p and q. */
            seq_t cand = concat_parts(p, q);

            /* Test the concatenated number for primality. */
            if (is_prime_big(cand)) {
                /* Store the new term. */
                ensure_terms_capacity(terms_len + 1);
                terms[terms_len++] = cand;
                last_p_chk = p + 1;   /* next search starts after p */
                break;               /* move to next required term */
            }
            /* If cand is not prime, continue searching for next p. */
        }
    }

    return terms[n - 1];
}
