/*
 * A124231  Numbers k such that pi(k) is palindromic, where pi(k) is the
 *          number of primes less than or equal to k.
 * https://oeis.org/A124231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = nth integer k such that pi(k) is palindromic
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"

/* Persistent state across calls. */
static seq_t *terms      = NULL;
static size_t terms_len  = 0;
static idx_t limit       = 0;

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for offset 0 – any value is OK */
        return 0;

    /* Grow the pre‑computed list until it contains at least n terms. */
    while (terms_len < (size_t)n) {
        idx_t new_limit;
        if (limit == 0) {
            new_limit = 1000;                  /* initial sieve limit   */
        } else {
            new_limit = limit * 2;             /* double each time       */
        }
        if (new_limit <= limit)                /* avoid overflow         */
            new_limit = limit + 1000;

        /* ---------- Sieve primes up to new_limit ---------- */
        size_t sz = (size_t)(new_limit + 1);
        bool *prime = (bool *)malloc(sz * sizeof(bool));
        if (!prime) return 0;

        for (size_t i = 0; i < sz; ++i) prime[i] = true;
        prime[0] = prime[1] = false;

        for (idx_t i = 2; i * i <= new_limit; ++i)
            if (prime[i])
                for (idx_t j = i * i; j <= new_limit; j += i)
                    prime[j] = false;

        /* ---------- Prefix prime‑count array ---------- */
        seq_t *pi = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!pi) { free(prime); return 0; }
        pi[0] = 0;
        for (idx_t i = 1; i <= new_limit; ++i)
            pi[i] = pi[i - 1] + (prime[i] ? 1 : 0);

        /* ---------- Collect terms with palindromic pi ---------- */
        seq_t *new_terms = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!new_terms) { free(prime); free(pi); return 0; }
        size_t count = 0;
        for (idx_t i = 1; i <= new_limit; ++i)
            if (is_palindrome(pi[i], (idx_t)10))
                new_terms[count++] = (seq_t)i;

        /* ---------- Replace old data ----------
           The previous arrays can be discarded after we finish using
           the newly built data. ----------
        */
        free(prime);
        free(pi);
        free(terms);

        terms      = new_terms;
        terms_len  = count;
        limit      = new_limit;
    }

    /* Return the requested term. */
    return terms[n - 1];
}
