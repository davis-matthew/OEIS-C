/*
 * A108936  Numbers n such that 11*n + 911 is prime.
 * https://oeis.org/A108936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A108936: numbers n such that 11*n + 911 is prime.
 * The function computes the n-th such number (offset 1) by
 * searching candidates in ascending order and storing results
 * in a static dynamic array.
 */

#include <stdbool.h>
#include <stdint.h>
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
    /* static state */
    static seq_t *values     = NULL; /* array of found n values */
    static idx_t  cap        = 0;    /* capacity of the array */
    static idx_t  count      = 0;    /* number of stored terms */
    static idx_t  next_test  = 0;    /* next candidate n to test */

    /* extend the list until it contains at least n terms */
    while (count < n)
    {
        /* compute 11*next_test + 911 safely */
        __uint128_t prod = (__uint128_t)11 * next_test + 911;
        /* if the value is larger than 64‑bit we cannot test it with is_prime */
        if (prod > UINT64_MAX)
            break;
        uint64_t val = (uint64_t)prod;
        if (is_prime_u64(val))
        {
            /* store the candidate as a seq_t */
            push_seq(&values, &cap, &count, (seq_t)next_test);
        }
        next_test++;
    }

    /* return the requested term (if available) */
    return (n <= count) ? values[n - 1] : (seq_t)0;
}
