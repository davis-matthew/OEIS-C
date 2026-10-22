/*
 * A003627  Primes of the form 3n-1.
 * https://oeis.org/A003627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: a(n) = n-th prime p with p == 2 (mod 3), i.e. p = 3k-1
 *            prime (found by trial division)
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a003627_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a003627_isprime.h"

static uint64_t *a003627_cache = NULL;
static idx_t a003627_len = 0;
static idx_t a003627_cap = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (a003627_len == 0) {
        a003627_cap = 64;
        a003627_cache = (uint64_t *)malloc(a003627_cap * sizeof(uint64_t));
        a003627_cache[0] = 2;
        a003627_len = 1;
    }
    while (a003627_len < n) {
        uint64_t c = a003627_cache[a003627_len - 1] + 3;
        if (a003627_len == 1) c = 5;
        while (!a003627_isprime(c)) c += 6 - 3; /* step through 3k-1 */
        if (a003627_len == a003627_cap) {
            a003627_cap *= 2;
            a003627_cache = (uint64_t *)realloc(a003627_cache, a003627_cap * sizeof(uint64_t));
        }
        a003627_cache[a003627_len++] = c;
    }
    return (seq_t)a003627_cache[n - 1];
}
