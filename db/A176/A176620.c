/*
 * A176620  Primes p for which the factorization of p! over distinct terms of
 *          A050376 does not contain 2.
 * https://oeis.org/A176620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 3683, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9452 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176620: primes p for which the exponent of 2 in p! is even.
   Computed by generating primes and accepting those where parity(p) == parity(popcount(p)). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Return true if the parity of p and the parity of its binary digit sum match. */
static inline bool even_v2_factorial(idx_t p)
{
    unsigned int pop_par = 0;          /* parity of popcount(p) */
    idx_t x = p;
    while (x) {
        pop_par ^= (x & 1);
        x >>= 1;
    }
    return ((p & 1) == pop_par);
}

/* Dynamic array of found primes */
static seq_t *list      = NULL;
static size_t list_cap  = 0;
static size_t list_len  = 0;
static idx_t  candidate = 3;          /* current odd candidate to test */

/* Append a value to the growing array, reallocating if needed. */
static void append_prime(seq_t val)
{
    if (list_len == list_cap) {
        size_t new_cap = (list_cap == 0) ? 16 : list_cap * 2;
        seq_t *tmp = mem_resize(seq_t, list, new_cap);
        if (!tmp) abort();
        list = tmp;
        list_cap = new_cap;
    }
    list[list_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0; harmless */

    /* Build the cache until we have at least n terms */
    while (list_len < n) {
        if (is_prime(candidate) && even_v2_factorial(candidate))
            append_prime((seq_t)candidate);
        candidate += 2;   /* skip even numbers, 2 is excluded automatically */
    }
    return list[n - 1];
}
