/*
 * A142534  Primes congruent to 4 mod 53.
 * https://oeis.org/A142534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 4 (mod 53), offset 1
 * Range:     u8 n <= 1, u16 n <= 129, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 863 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    static idx_t candidate = 4;          // last checked candidate (starts at 4)
    static seq_t *cache      = NULL;     // array of found primes
    static size_t count     = 0;         // number of primes stored
    static size_t cap       = 0;         // current capacity of cache

    if (n == 0) return 0;               // undefined for n < 1, return 0

    if ((idx_t)n <= count) return cache[n - 1];

    /* Generate more primes until we have at least n of them */
    for (;;) {
        candidate += 53;                 /* next number ≡ 4 (mod 53) */
        if (is_prime(candidate)) {
            if (count >= cap)
                grow_seq(&cache, &cap, count + 1);
            cache[count++] = (seq_t)candidate;
            if (count >= n) break;
        }
    }
    return cache[n - 1];
}
