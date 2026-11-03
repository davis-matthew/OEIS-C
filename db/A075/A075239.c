/*
 * A075239  Primes whose base 9 reversal is also prime.
 * https://oeis.org/A075239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1883, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9661 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075239 – primes whose base‑9 reversal is also prime.
   The function incrementsally builds the list of such primes.
   It stores the terms in a static dynamic array and only generates
   new terms when requested.  The reverse of a number is computed
   in base 9, then the resulting decimal integer is checked for
   primality as well. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Reverse the base‑9 representation of n and return the
   resulting decimal integer (as a 64‑bit value). */
static inline uint64_t reverse_base9(uint64_t n)
{
    uint64_t rev = 0;
    while (n > 0) {
        rev = rev * 9 + (n % 9);
        n /= 9;
    }
    return rev;
}

/* Append a new term to the cache, resizing if necessary. */
static void append_term(seq_t val, seq_t **cache, size_t *len, size_t *cap)
{
    if (*len == *cap) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 8;
        seq_t *tmp = mem_resize(seq_t, *cache, new_cap);
        if (!tmp) abort();
        *cache = tmp;
        *cap   = new_cap;
    }
    (*cache)[*len] = val;
    (*len)++;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache of terms already found */
    static seq_t *terms   = NULL;
    static size_t term_cnt = 0;
    static size_t term_cap = 0;

    /* The next integer to test for primeness */
    static uint64_t candidate = 2;

    /* Generate terms until the cache contains at least n entries */
    while (term_cnt < (size_t)n) {
        if (is_prime(candidate)) {
            uint64_t rev = reverse_base9(candidate);
            if (is_prime(rev)) {
                append_term((seq_t)candidate, &terms, &term_cnt, &term_cap);
            }
        }
        candidate++;
    }

    return terms[n - 1];
}
