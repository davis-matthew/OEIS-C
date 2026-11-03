/*
 * A117689  Cubes for which the product of the digits is a square.
 * https://oeis.org/A117689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 945, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 9813 (2026-11-01)
 * Helpers:   digit_product, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th cube whose digits product is a perfect square
   (OEIS A117689).  The sequence is generated on demand, caching
   results across calls. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_product.h"
#include "../utils/ipow_i32.h"

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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
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

/* static cache of computed terms */
static seq_t *a_cache = NULL;
static size_t a_cap   = 0;      /* allocated capacity of a_cache      */
static idx_t  a_count = 0;      /* number of terms stored in a_cache  */
static idx_t  last_k  = 0;      /* next cube root to test  */

/* Return true iff x is a perfect square. */
static bool is_square(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

/* Grow a_cache to at least new_cap elements.  Uses seq_realloc so
   that any non‑trivial seq_t type is constructed properly. */
static void ensure_cache_cap(size_t new_cap)
{
    if (new_cap <= a_cap) return;
    a_cache = seq_realloc(a_cache, a_cap, new_cap);
    a_cap   = new_cap;
}

/* Return the n-th term of sequence A117689.  The first few terms:
   a(0)=0, a(1)=1, a(2)=343, … . */
seq_t a(idx_t n)
{
    while (a_count <= n)
    {
        /* Compute last_k³ safely using mul_small to avoid overflow */
        seq_t sq   = mul_small((seq_t)last_k, (unsigned int)last_k);
        seq_t cube = mul_small(sq, (unsigned int)last_k);

        seq_t prod = digit_product(cube);
        if (is_square(prod))
        {
            ensure_cache_cap(a_count + 1);
            a_cache[a_count++] = cube;
        }
        ++last_k;
    }
    return a_cache[n];
}
