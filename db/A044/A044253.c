/*
 * A044253  Numbers n such that string 0,2 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth integer n such that the base‑9 representation
   of n contains the substring "0,2" (digit 0 followed immediately by 2)
   but the base‑9 representation of n‑1 does not. */
/* OEIS A044253 */
#include <stddef.h>
#include <stdbool.h>

/* Sufficient size to cache many terms; grows automatically if needed. */
#define CACHE_INIT_SIZE 2000          /* initial capacity */
static seq_t *cache = NULL;
static size_t cap = 0;             /* current capacity of cache   */
static size_t cnt = 0;             /* number of terms stored      */
static seq_t search_n = 0;         /* last number examined        */

/* Test whether the base‑9 expansion of x contains the substring 0,2
   when read from most significant digit to least.  */
static bool has02(seq_t x)
{
    if (x < (seq_t)9)                /* single‑digit numbers cannot contain it */
        return false;

    unsigned int prev = 255;         /* sentinel value ≠ 0,2 */
    while (x > (seq_t)0) {
        unsigned int curr = (unsigned int)(x % (seq_t)9);   /* LSB digit */
        if (prev == 2 && curr == 0)   /* pattern 0,2 seen in normal order */
            return true;
        prev = curr;
        x /= (seq_t)9;
    }
    return false;
}

/* Resize cache if needed. */
static void ensure_cache(size_t desired)
{
    if (cap >= desired)
        return;
    size_t new_cap = cap ? cap * 2 : CACHE_INIT_SIZE;
    if (new_cap < desired)
        new_cap = desired;
    seq_t *new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_ptr)
        return;                   /* memory error – will continue with existing cache */
    if (cache) {
        for (size_t i = 0; i < cnt; ++i)
            new_ptr[i] = cache[i];
        free(cache);
    }
    cache = new_ptr;
    cap = new_cap;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n < 1 */
        return (seq_t)0;

    ensure_cache((size_t)n);
    if (cnt >= (size_t)n)
        return cache[(size_t)n - 1U];

    /* Search forward until we have enough terms. */
    while (cnt < (size_t)n) {
        ++search_n;
        /* Check pattern at search_n and at search_n‑1.  search_n >= 1 here. */
        if (has02(search_n) && !has02(search_n - (seq_t)1)) {
            cache[cnt++] = search_n;
        }
    }
    return cache[(size_t)n - 1U];
}
