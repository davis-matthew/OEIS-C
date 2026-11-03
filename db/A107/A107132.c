/*
 * A107132  Primes of the form 2x^2 + 13y^2.
 * https://oeis.org/A107132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 552, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9767 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 2x^2 + 13y^2.
   The function searches primes in increasing order and keeps them in
   a memoized static array. Representation testing is done with a
   simple quadratic check. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"
#include "../utils/mem.h"

/* Append a value to a dynamic array of seq_t. */
static void add_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size + 1 > *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap * 2);
        *arr = mem_resize(seq_t, *arr, newcap);
        *cap = newcap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

/* Test whether a prime p can be written as 2x^2 + 13y^2. */
static bool has_rep(uint64_t p)
{
    /* y^2 must be <= p/13 */
    uint64_t max_y = isqrt_u64(p / 13);
    for (uint64_t y = 0; y <= max_y; ++y) {
        uint64_t rem = p - 13 * y * y;       /* even if representable */
        if (rem & 1ULL) continue;           /* must be even */
        uint64_t s = rem >> 1;
        uint64_t t = isqrt_u64(s);
        if (t * t == s) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Memoized list of primes of the form 2x^2 + 13y^2. */
    static seq_t *list = NULL;
    static idx_t list_cap = 0, list_size = 0;
    static uint64_t next_candidate = 3;   /* start after prime 2 */

    /* Initialize the list with the first prime of the form. */
    if (list == NULL) {
        list_cap = 16;
        list = mem_new(seq_t, list_cap);
        add_seq(&list, &list_cap, &list_size, (seq_t)2);
    }

    /* Generate further terms until we have at least n terms. */
    while (list_size < n) {
        /* Skip even numbers after 2. */
        while (next_candidate % 2ULL == 0) next_candidate++;

        if (is_prime_u64(next_candidate) && has_rep(next_candidate))
            add_seq(&list, &list_cap, &list_size, (seq_t)next_candidate);

        next_candidate += 2;
    }

    return list[n - 1];
}
