/*
 * A191031  Primes that are squares mod 43.
 * https://oeis.org/A191031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n‑th prime that is a quadratic residue modulo 43
    (i.e. primes that split in Q(√-43)).  The sequence is generated
    by a simple sieve of Eratosthenes combined with the Legendre symbol
    (Jacobi symbol over modulus 43).  The result is memoised so that
    repeated calls return instantly after the table has been built.  */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "../utils/jacobi_symbol.h"

/* dynamic array helpers */
static void grow_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* global state for memoised results */
static seq_t *target_primes = NULL;
static size_t tgt_sz = 0, tgt_cap = 0;
static uint64_t processed_up_to = 0;

/* add primes satisfying the condition within a given limit */
static void generate_up_to(uint64_t new_limit)
{
    if (new_limit <= processed_up_to) return;

    /* sieve of Eratosthenes up to new_limit */
    uint8_t *is_composite = (uint8_t*)calloc((size_t)new_limit + 1, 1);
    if (!is_composite) return;               /* allocation failure: ignore */

    for (uint64_t i = 2; i * i <= new_limit; ++i) {
        if (!is_composite[i]) {
            for (uint64_t j = i * i; j <= new_limit; j += i)
                is_composite[j] = 1;
        }
    }

    for (uint64_t p = processed_up_to + 1; p <= new_limit; ++p) {
        if (is_composite[p]) continue;          /* not prime */

        if (p == 43) continue;                  /* ramified prime */

        bool ok = false;
        if (p < 43) {                           /* special small case   */
            uint64_t val = 4 * p - 43;           /* 4p = 43 + x^2           */
            uint64_t r = (uint64_t)(sqrt((double)val) + 0.5);
            if (r * r == val) ok = true;
        } else {                                 /* Legendre symbol (p|43)  */
            if (jacobi_symbol(p, 43) == 1) ok = true;
        }

        if (ok) {
            grow_array(&target_primes, &tgt_cap, tgt_sz + 1);
            target_primes[tgt_sz++] = (seq_t)p;
        }
    }

    free(is_composite);
    processed_up_to = new_limit;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n < 1 */

    /* progressively enlarge sieve until we have at least n terms */
    while (tgt_sz < (size_t)n) {
        uint64_t limit = processed_up_to < 32 ? 32 : processed_up_to * 2;
        if (limit < processed_up_to) limit = UINT64_MAX; /* overflow guard */
        generate_up_to(limit);
    }

    return target_primes[n - 1];
}
