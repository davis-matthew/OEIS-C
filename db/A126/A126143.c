/*
 * A126143  Primes whose sum of the decimal digits of their cubes is also a
 *          prime.
 * https://oeis.org/A126143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the n‑th prime p such that the sum of the decimal digits
 *            of p³ is also a prime
 * Range:     u8 none, u16 n <= 4, u32 n <= 100, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   big_pow, ensure_capacity, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_prime_small.h"
#include "../utils/big_pow.h"
#include "../utils/ensure_capacity.h"

/* ---------- static state ---------- */
static seq_t *terms      = NULL;   /* array of found terms */
static size_t terms_len  = 0;      /* number of stored terms    */
static size_t terms_cap  = 0;      /* allocated capacity       */
static idx_t  last_checked = 0;    /* last integer examined    */

/* Expand the terms array so it can store at least 'need' elements. */
static void ensure_terms_cap(void)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        ensure_capacity(new_cap, &terms_cap, (void **)&terms, sizeof(seq_t));
    }
}

/* Compute the sum of decimal digits of a seq_t value.  The result
   fits into a native integer but we keep it in seq_t for uniformity. */
static seq_t digit_sum(seq_t v)
{
    seq_t sum = 0;
    while (v > 0) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* Return the n‑th term of sequence A126143.  a(1) is the first
   prime satisfying the condition.  For n = 0 we return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but handled gracefully */

    /* Generate terms until at least n are found. */
    while (terms_len < n) {
        /* Find the next integer > last_checked that may be prime. */
        idx_t cand_i;
        do {
            ++last_checked;
            cand_i = last_checked;
        } while (cand_i > 2 && (cand_i & 1) == 0); /* skip evens after 2 */

        seq_t cand = (seq_t)cand_i;

        /* Test whether cand is prime.  is_prime_big works for any seq_t. */
        if (!is_prime_big(cand))
            continue;  /* not prime – keep searching */

        /* Sum of decimal digits of the cube. */
        seq_t cube = big_pow(cand, 3);
        seq_t s    = digit_sum(cube);

        /* If that sum is also prime -> store the prime. */
        if (is_prime_small(s)) {
            ensure_terms_cap();
            terms[terms_len++] = cand;
        }
    }

    return terms[n - 1];
}
