/*
 * A021622  Decimal expansion of 1/618.
 * https://oeis.org/A021622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A021622 – Decimal expansion of 1/618
 *  Computes the nth digit after the decimal point of 1/618
 *  using long‑division.  The digits are stored in a dynamically
 *  sized cache so the function works for any order of calls.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
/* In C++ mode we can use the native new/delete operators.  */
static seq_t *digit_cache = nullptr;
#else
static seq_t *digit_cache = NULL;
#endif
/* Number of digits that have already been generated.  */
static idx_t cache_size = 0;
/* Capacity of the allocation backing digit_cache.  */
static size_t cache_capacity = 0;
/* Remainder used by the long division process (0 <= rem < 618).  */
static uint64_t rem = 1ULL;

/*----------------------------------------------------------------------------*/
static void ensure_capacity(idx_t need)
/* Make sure the cache can hold digits up to index 'need'.  */
{
    if ((size_t)need < cache_capacity)
        return;
    size_t new_capacity = cache_capacity ? cache_capacity << 1U : 1U;
    while (new_capacity <= (size_t)need)
        new_capacity <<= 1U;
#if defined(__cplusplus)
    seq_t *newptr = new seq_t[new_capacity];
#else
    seq_t *newptr = (seq_t *)realloc(digit_cache, new_capacity * sizeof(seq_t));
#endif
    /* Nothing else to copy for the very first allocation.  */
    for (idx_t i = 0; i < cache_size; ++i)
        newptr[i] = digit_cache[i];
#if defined(__cplusplus)
    delete[] digit_cache;
#else
    /* In C mode, realloc already freed the old block.  */
#endif
    digit_cache = newptr;
    cache_capacity = new_capacity;
}

/*----------------------------------------------------------------------------*/
static void build_cache_to(idx_t n)
/* Generate digits up to index n (inclusive) using long division.  */
{
    ensure_capacity(n);
    for (idx_t i = cache_size; i <= n; ++i)
    {
        uint64_t temp = rem * 10ULL;
        uint64_t d    = temp / 618ULL;
        rem          = temp % 618ULL;
        digit_cache[i] = (seq_t)d;  /* implicit conversion to seq_t */
    }
    cache_size = n + 1U;
}

/*----------------------------------------------------------------------------*/
seq_t a(idx_t n)
/* Return the nth digit after the decimal point of 1/618.  */
{
    if (n >= cache_size)
        build_cache_to(n);
    return digit_cache[n];
}
