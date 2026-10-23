/*
 * A107198  Primes of the form x^2 + 84y^2.
 * https://oeis.org/A107198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form x^2 + 84y^2 (OEIS A107198) */
/* a(1) = 109, a(2) = 337, … */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime64.h"

/* Integer square root for 64‑bit values (returns floor(sqrt(x))) */
static uint64_t isqrt64(uint64_t x)
{
    uint64_t r = 0;
    int shift = 0;
    while ((uint64_t)1 << (shift + 1) <= x) shift++;
    for (; shift >= 0; shift--)
    {
        uint64_t t = r | ((uint64_t)1 << shift);
        if (t <= x / t) r = t;
    }
    return r;
}

/* Check if the prime p can be written as x^2 + 84*y^2. */
static bool has_representation(uint64_t p)
{
    uint64_t max_y = isqrt64(p / 84ULL);
    for (uint64_t y = 0; y <= max_y; ++y)
    {
        __int128 rem = (__int128)p - (__int128)84 * y * y;
        if (rem < 0) break;
        uint64_t r = isqrt64((uint64_t)rem);
        if ((__int128)r * r == rem) return true;
    }
    return false;
}

/* Dynamic storage of the sequence terms */
static seq_t *terms_arr = NULL;
static size_t terms_cap = 0;
static size_t terms_cnt = 0;
static uint64_t candidate = 2;  /* next integer to test for primality */

/* Append a new term to the array, resizing if necessary. */
static void add_term(seq_t val)
{
    if (terms_cnt == terms_cap)
    {
        size_t new_cap = terms_cap ? terms_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(terms_arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        terms_arr = tmp;
        terms_cap = new_cap;
    }
    terms_arr[terms_cnt++] = val;
}

/* Generate terms until we have at least n of them. */
static void ensure_terms(idx_t n)
{
    if (terms_cnt >= (size_t)n) return;
    while (terms_cnt < (size_t)n)
    {
        if (isPrime64(candidate) && has_representation(candidate))
            add_term((seq_t)candidate);
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. Behavior for n = 0 is undefined. */
    ensure_terms(n);
    return terms_arr[n - 1];
}
