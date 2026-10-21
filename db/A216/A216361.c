/*
 * A216361  a(n) = (A216315(n) - 1)/118.
 * https://oeis.org/A216361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = k such that 118*k+1 is prime (OEIS A216361) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *k_arr  = NULL;   /* stored k values */
static size_t stored  = 0;      /* number of values stored */
static size_t capacity = 0;     /* allocated capacity of k_arr */
static idx_t next_k  = 1;       /* next k candidate to test */

/* Ensure that k_arr has at least min_cap entries allocated. */
static void ensure_cap(size_t min_cap)
{
    if (min_cap <= capacity)
        return;
    size_t new_cap = capacity ? capacity * 2 : 16;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)                         /* abort on OOM */
        exit(EXIT_FAILURE);
    if (k_arr) {
        for (size_t i = 0; i < stored; ++i)
            new_arr[i] = k_arr[i];
        free(k_arr);
    }
    k_arr    = new_arr;
    capacity = new_cap;
}

/* Generate k values until we have at least needed stored values. */
static void generate_to(size_t needed)
{
    while (stored < needed) {
        idx_t p = (idx_t)118 * next_k + 1;
        if (is_prime(p)) {
            ensure_cap(stored + 1);
            k_arr[stored] = (seq_t)next_k;
            ++stored;
        }
        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* undefined for n==0, return 0 */
    generate_to((size_t)n);
    return k_arr[n - 1];
}
