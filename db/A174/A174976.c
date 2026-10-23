/*
 * A174976  Primes which have an equal number of 0,1,2 in their Base_3
 *          expansion.
 * https://oeis.org/A174976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   equal_012, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes whose ternary expansion has equal numbers of 0, 1, 2. */
/* The function keeps a static cache of already‑found primes and extends it
 * on demand.  The test for primality uses is_prime(idx_t).  All arithmetic
 * is performed on idx_t (uint64_t) values; the result is returned as seq_t. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/equal_012.h"

/* Cache of primes that satisfy the ternary digit property */
static idx_t current      = 2;          /* next candidate to test     */
static idx_t *specials    = NULL;       /* array of found primes      */
static size_t specials_cap = 0;         /* allocated capacity         */
static size_t specials_len = 0;         /* number of stored primes    */

/* Ensure the specials array has enough capacity. */
static void ensure_specials_cap(void) {
    if (specials_len == specials_cap) {
        size_t new_cap = specials_cap == 0 ? 16 : specials_cap * 2;
        idx_t *tmp = (idx_t *)realloc(specials, new_cap * sizeof(idx_t));
        specials = tmp;                  /* realloc returns void* in C++ */
        specials_cap = new_cap;
    }
}

/* Append a qualifying prime to the specials cache. */
static void add_special(idx_t val) {
    ensure_specials_cap();
    specials[specials_len++] = val;
}

/*  Sequence function.  idx_t n is 1‑based; behaviour for n==0 is undefined
 *  but a safe fallback return value is provided. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* safe fallback; offset=1 */

    /* Grow the cache until it contains at least n elements. */
    while (specials_len < n) {
        if (is_prime(current) && equal_012(current))
            add_special(current);

        /* Move to the next candidate */
        if (current <= 2)
            current = 3;
        else
            current += 2; /* skip even numbers */
    }

    /* Return the requested prime as seq_t. */
    return (seq_t)specials[n - 1];
}
