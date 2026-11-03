/*
 * A140349  A number k is included if both (the largest divisor of k that is
 *          <= sqrt(k)) and (the smallest divisor of k that is >= sqrt(k)) are
 *          composite.
 * https://oeis.org/A140349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 2687, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 8720 (2026-11-01)
 * Helpers:   ipow_i32, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A140349: a(n) is the nth positive integer k such that
   the largest divisor of k that is ≤ sqrt(k) and
   the smallest divisor of k that is ≥ sqrt(k) are both composite. */
/* The function generates the sequence by brute‑force search. */
/* The implementation works for any seq_t and idx_t types supplied by the build. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_seq.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static inline bool is_composite(seq_t v)
{
    return v > 1 && !is_prime_seq(v);
}

/* Test whether k belongs to the sequence. */
static bool in_sequence(seq_t k)
{
    if (k < 4) /* the smallest term is 16 */
        return false;

    seq_t s = floor_sqrt(k);          /* floor(sqrt(k)) */
    seq_t d1 = 0;                     /* largest divisor ≤ sqrt(k) */

    for (seq_t d = s; d >= 2; --d)
    {
        if (k % d == 0)
        {
            d1 = d;
            break;
        }
    }

    if (d1 == 0) /* k is prime; no divisor ≤ sqrt(k) > 1 */
        return false;

    seq_t d2 = k / d1;                 /* complementary divisor */
    return is_composite(d1) && is_composite(d2);
}

/* Ensure that the array has capacity at least new_cap elements. */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t new_cap)
{
    if (new_cap <= *cap)
        return;
    seq_t *new_arr = seq_realloc(*arr, *cap, new_cap);
    *arr = new_arr;
    *cap = new_cap;
}

/* External interface required by the framework. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;      /* cached terms */
    static size_t size = 0;          /* number of terms stored */
    static size_t cap = 0;           /* current allocation capacity */
    static seq_t cur_k = 0;           /* largest k already tested */

    /* Initialise storage if first call. */
    if (terms == NULL)
    {
        cap = 1;
        terms = seq_alloc(cap);
    }

    /* Search until we have at least n terms. */
    while (size < (size_t)n)
    {
        ++cur_k;
        if (in_sequence(cur_k))
        {
            ensure_capacity(&terms, &cap, size + 1);
            terms[size++] = cur_k;
        }
    }

    return terms[(size_t)n - 1];
}
