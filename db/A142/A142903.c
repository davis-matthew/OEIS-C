/*
 * A142903  Primes congruent to 25 mod 63.
 * https://oeis.org/A142903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 179, u32 n <= 119, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 25 (mod 63) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/*  a(n) = the nth prime p such that p ≡ 25 (mod 63)  (n ≥ 1)  */
seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *terms      = NULL;   /* stored primes */
    static size_t  terms_cap = 0;      /* allocated capacity */
    static idx_t   terms_cnt = 0;      /* number of primes stored */
    static idx_t   cand       = 0;      /* current candidate (∈ 25 + 63·ℕ) */
    static bool    init       = false; /* initialization flag */

    /* initialise on first call */
    if (!init) {
        init = true;
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) return (seq_t)0;              /* allocation failure */
        terms_cnt = 0;
        cand = (idx_t)63 * 2 + 25;                /* first potential prime */
    }

    /* if already have enough terms, return the required one */
    if (terms_cnt >= n) {
        if (n == 0) return (seq_t)0;              /* out of bounds, but safe */
        return terms[n - 1];
    }

    /* generate further terms until we reach the requested index */
    while (terms_cnt < n) {
        if (is_prime_big((seq_t)cand)) {
            /* grow the array if needed */
            if (terms_cnt == terms_cap) {
                terms_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;        /* reallocation failure */
                terms = tmp;
            }
            terms[terms_cnt++] = (seq_t)cand;
        }
        cand += (idx_t)63;  /* next number in the progression 25 + 63·k */
    }

    return terms[n - 1];
}
