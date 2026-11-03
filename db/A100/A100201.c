/*
 * A100201  Primes of the form 23*k+3.
 * https://oeis.org/A100201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p ≡ 3 (mod 23)
 * Range:     u8 n <= 2, u16 n <= 301, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9573 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;  /* array of found primes */
    static idx_t   size   = 0;     /* number of stored primes   */
    static idx_t   cap    = 0;     /* allocated capacity       */
    static uint64_t next_k = 0;    /* next k to test for 23*k+3 */

    /* Generate primes until we have at least n of them. */
    while (size < n) {
        uint64_t candidate = 23 * next_k + 3;   /* candidate prime */
        if (is_prime_u64(candidate))
            push_seq(&primes, &cap, &size, (seq_t)candidate);
        ++next_k;
    }

    /* Return the (n‑1)th element (the nth prime in 1‑based indexing). */
    return primes[n - 1];
}
