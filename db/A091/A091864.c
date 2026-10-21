/*
 * A091864  Positive integers n such that Sum_{p<=pi(n)} p = n, where pi(x)
 *          denotes the number of primes not exceeding x and the summation is
 *          over primes p.
 * https://oeis.org/A091864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the nth natural number n such that sum_{p <= pi(n)} p =
 *            n, where pi is the prime‑counting function (search). The
 *            algorithm iterates over k = pi(n); for each k it maintains the
 *            sum of all primes ≤ k. Whenever that sum s has pi(s) = k, the
 *            number s is a member of the sequence
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   compute_pi, is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/compute_pi.h"
#include "../utils/seq_realloc.h"

/* static cache of found terms */
static seq_t *terms = NULL;
static size_t terms_cnt = 0;
static size_t terms_cap = 0;

/* Ensure that the terms array has at least 'need' capacity.
   Reallocates when necessary. */
static void ensure_terms_capacity(size_t need)
{
    if (terms_cap >= need)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    terms = seq_realloc(terms, terms_cap, new_cap);
    terms_cap = new_cap;
}

/* Return the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* generate terms until we have at least n of them */
    static seq_t sum = 0;          /* sum of primes <= current k */
    static idx_t k = 1;           /* current prime count threshold */

    while (terms_cnt < (size_t)n) {
        /* move to the next integer k */
        k++;

        /* add k to the sum if it is a prime */
        if (is_prime(k))
            sum = sum + (seq_t)k;

        /* check if the sum satisfies the property */
        seq_t pi_val = compute_pi((idx_t)sum);
        if (pi_val == (seq_t)k) {
            ensure_terms_capacity(terms_cnt + 1);
            terms[terms_cnt++] = sum;
        }
    }
    return terms[n - 1];
}
