/*
 * A044632  Numbers n such that string 0,0 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044632
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

/* A044632: numbers n such that the base‑9 representation of n contains
   the consecutive digits "00" but the representation of n+1 does not.
   The function computes the n‑th such number by a brute‑force scan. */

#include <stdbool.h>
#include <stddef.h>
#ifdef __cplusplus
#  include <new>   /* for operator new/delete */
#endif

/* Forward declaration of helpers */
static bool has_consecutive_zeros(seq_t);

/* Cache for already found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* capacity of the cache array */
static seq_t next_candidate = 1;  /* next integer to test */

/* Ensure the cache can hold at least 'needed' elements.  Copy existing
   entries to the new storage when resizing. */
static void cache_reserve(size_t needed)
{
    if (needed <= cache_cap) return;

    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    if (newcap < needed) newcap = needed;

#ifdef __cplusplus
    seq_t *newp = new seq_t[newcap];
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i) newp[i] = cache[i];
        delete[] cache;
    }
    cache = newp;
#else
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
#endif
    cache_cap = newcap;
}

/* Check whether a non‑negative integer n has a consecutive pair of zeros
   in its base‑9 representation.  The function only uses the operators
   allowed for seq_t. */
static bool has_consecutive_zeros(seq_t n)
{
    bool prev_zero = false;
    while (n > 0) {
        seq_t d = n % 9;
        if (prev_zero && d == 0) return true;
        prev_zero = (d == 0);
        n = n / 9;
    }
    return false;
}

/* Public interface required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1, below offset not used */

    while (cache_len < n) {
        seq_t x = next_candidate;

        if (has_consecutive_zeros(x) && !has_consecutive_zeros(x + 1)) {
            cache_reserve(cache_len + 1);
            cache[cache_len++] = x;
        }

        next_candidate++;
    }

    return cache[n - 1];
}
