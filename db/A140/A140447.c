/*
 * A140447  List of prime pairs of form p, p+22.
 * https://oeis.org/A140447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th element of the interleaved list of primes p
 *            such that both p and p+22 are prime. The odd positions contain
 *            p, the even positions contain p+22
 * Range:     u8 n <= 27, u16 n <= 1935, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* See OEIS A140447 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Static storage for the generated terms */
static seq_t *terms      = NULL;   /* array holding the sequence values     */
static size_t terms_cap  = 0;      /* allocated size of terms[]            */
static size_t terms_len  = 0;      /* number of terms currently stored     */
static idx_t  last_checked = 0;    /* last integer that was checked       */

/* Ensure the terms array has space for at least 'need' more elements. */
static void ensure_terms_capacity(size_t need)
{
    if (terms_len + need <= terms_cap) return;

    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < terms_len + need) new_cap = terms_len + need;

    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);              /* fatal error handling */
    terms      = tmp;
    terms_cap  = new_cap;
}

/* Return the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety – the sequence is 1‑based */

    /* Generate terms until we have at least 'n' of them. */
    while (terms_len < (size_t)n) {
        /* Find the next prime p greater than last_checked. */
        idx_t p = last_checked + 1;
        for ( ; ; ++p) {
            if (is_prime(p)) {
                last_checked = p;
                break;
            }
        }

        /* Check whether p + 22 is also prime. */
        idx_t q = p + 22;
        if (q < (idx_t)-1 && is_prime(q)) {
            /* Store the pair p, q consecutively. */
            ensure_terms_capacity(2);
            terms[terms_len++] = (seq_t)p;
            terms[terms_len++] = (seq_t)q;
        }

        /* If the current candidate did not produce a pair, simply
           continue the loop to find the next prime. */
    }

    return terms[n - 1];
}
