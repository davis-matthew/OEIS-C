/*
 * A142086  Primes congruent to 16 mod 35.
 * https://oeis.org/A142086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 16 (mod 35)
 * Range:     u8 n <= 1, u16 n <= 275, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 941 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    static seq_t *primes = NULL;
    static size_t count   = 0;
    static size_t capacity = 0;
    static idx_t candidate = 16;
    static int initialized = 0;

    if (!initialized) {
        initialized = 1;
        candidate = 16;
    }

    while (count < (size_t)n) {
        candidate += 35;
        if ((candidate & 1U) == 0) continue;           /* skip even numbers */
        if (is_prime(candidate)) {
            grow_seq(&primes, &capacity, count + 1);
            primes[count++] = (seq_t)candidate;
        }
    }
    return primes[n - 1];
}
