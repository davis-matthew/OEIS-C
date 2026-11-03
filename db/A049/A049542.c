/*
 * A049542  Primes p such that x^10 = 2 has a solution mod p.
 * https://oeis.org/A049542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that x^10 ≡ 2 (mod p) has a solution
 *            (i.e. 2 is a 10th power residue modulo p). The sequence is
 *            computed by testing primes in increasing order and selecting
 *            those that satisfy the condition 2^((p-1)/gcd(10,p-1)) ≡ 1 (mod
 *            p). The array of found primes is cached so that...
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 988 (2026-11-01)
 * Helpers:   gcd_i32, next_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helpers for modular exponentiation, next prime, integer GCD
   and dynamic array allocation. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/gcd_i32.h"
#include "../utils/next_prime.h"
#include "../utils/powmod.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *special = NULL;         /* cached list of suitable primes */
static size_t special_cap = 0;        /* capacity of the array */
static size_t special_sz  = 0;        /* number of elements stored */
static uint64_t cur_prime = 2;        /* last prime tested (starts at 2) */

/* Ensure the array can hold at least min_size elements. */
static void ensure_capacity(size_t min_size) {
    if (special_cap < min_size) {
        size_t new_cap = special_cap ? special_cap * 2 : 16;
        if (new_cap < min_size) new_cap = min_size;
        if (special == NULL) {
            special = seq_alloc(new_cap);
        } else {
            special = seq_realloc(special, special_cap, new_cap);
        }
        special_cap = new_cap;
    }
}

/* Test whether a prime p satisfies the 10th‑power residue condition.
   For odd primes the condition is 2^((p-1)/gcd(10,p-1)) ≡ 1 (mod p).
   The prime 2 is handled separately (the equation has a solution x=0). */
static bool good_prime(uint64_t p) {
    if (p == 2) return true;          /* trivial solution modulo 2 */
    int g = gcd_i32(10, (int)(p - 1));
    uint64_t exp = (p - 1) / g;
    seq_t rem = powmod((seq_t)2, exp, (seq_t)p);
    return rem == (seq_t)1;
}

seq_t a(idx_t n) {
    /* n is 1‑based.  Fetch from cache if possible. */
    if ((size_t)n <= special_sz) {
        return special[(size_t)n - 1];
    }

    /* Expand the search until we have at least n qualifying primes. */
    while (special_sz < (size_t)n) {
        uint64_t p = cur_prime;          /* current prime candidate */

        /* Test the current prime */
        if (good_prime(p)) {
            ensure_capacity(special_sz + 1);
            special[special_sz++] = (seq_t)p;
        }

        /* Prepare the next prime for the next iteration. */
        cur_prime = next_prime(cur_prime);
    }

    return special[(size_t)n - 1];
}
