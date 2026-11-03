/*
 * A142277  Primes congruent to 28 mod 43.
 * https://oeis.org/A142277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 988 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth prime congruent to 28 modulo 43 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    /* static cache of found primes in the required residue class */
    static seq_t *primes = NULL;
    static idx_t count = 0;
    static idx_t cap   = 0;
    static idx_t next_k = 1;  /* next value of k for 28 + 43*k */

    /* generate primes until we have at least n of them */
    while (count < n) {
        seq_t candidate = (seq_t)28 + (seq_t)43 * (seq_t)next_k;
        next_k++;
        if (is_prime((idx_t)candidate)) {
            /* enlarge the array if necessary */
            grow_seq(&primes, &cap, count + 1);
            primes[count++] = candidate;
        }
    }

    return primes[n - 1];
}
