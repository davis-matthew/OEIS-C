/*
 * A107159  Primes of the form 4x^2 + 13y^2.
 * https://oeis.org/A107159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 4x^2 + 13y^2 (searched explicitly)
 * Range:     u8 n <= 6, u16 n <= 810, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9171 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt.h"
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

/* Helper: true if n is a perfect square (uint64_t) */
static bool is_square_uint64(uint64_t n)
{
    uint64_t r = isqrt(n);
    return r * r == n;
}

/* Helper: true if p can be written as 4x^2 + 13y^2   (p is prime) */
static bool represented(uint64_t p)
{
    /* y may start from 0; y^2 <= p/13 */
    uint64_t maxy = isqrt(p / 13);
    for (uint64_t y = 0; y <= maxy; ++y) {
        uint64_t term  = 13 * y * y;
        if (term > p) break;
        uint64_t rem = p - term;
        /* rem must be divisible by 4 */
        if (rem & 3) continue;                 /* rem % 4 != 0 */
        uint64_t x2 = rem >> 2;                 /* rem / 4 */
        if (is_square_uint64(x2)) return true;
    }
    return false;
}

/* The sequence function */
seq_t a(idx_t n)
{
    static seq_t *arr = NULL;     /* dynamic array of found primes   */
    static idx_t  cap = 0;        /* current capacity of arr        */
    static idx_t  size = 0;       /* number of elements stored      */
    static uint64_t candidate = 5;/* last odd number tested (start) */

    /* Grow the array while we don't have n terms yet */
    while (size < n) {
        /* Find the next prime */
        for (;;) {
            if (is_prime_u64(candidate)) break;
            candidate += 2;               /* skip even numbers */
        }

        /* Check if the prime is of the required form */
        if (represented(candidate)) {
            push_seq(&arr, &cap, &size, (seq_t)candidate);
        }

        candidate += 2;                   /* advance to next odd */
    }

    return arr[n - 1];                     /* 1‑based indexing */
}
