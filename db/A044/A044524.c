/*
 * A044524  Numbers n such that string 0,5 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A044524: numbers n whose base‑7 representation
   contains the substring 0,5 but whose successor n+1 does not. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Forward declaration of the required function */
seq_t a(idx_t n);

/* ------------------------------------------------------------ */
/* Test whether a base‑7 number contains the consecutive digits
   0 followed by 5 (i.e., ... 0 5 ...).  The digits are examined
   from least significant upward; a pair of digits (high, low)
   corresponds to the values d1 (higher) and d0 (lower).  */
static inline bool has05(seq_t x)
{
    while (x >= 7) {
        seq_t d0 = x % 7;   /* least significant digit of current pair */
        x /= 7;
        seq_t d1 = x % 7;   /* next higher digit of current pair */
        if (d0 == 5 && d1 == 0)  /* low digit 5, high digit 0 */
            return true;
    }
    return false;
}

/* ------------------------------------------------------------ */
/* Cache to store the sequence values as they are generated.  */
static seq_t *cache      = NULL;   /* array of found terms          */
static size_t cache_len  = 0;      /* number of terms stored        */
static size_t cache_cap  = 0;      /* allocated capacity           */
static idx_t  last_checked = 0;    /* last integer examined        */

/* Ensure that the cache has at least \"cap\" capacity.  */
static void ensure_cache_capacity(void)
{
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap <<= 1;                         /* double the capacity */
        seq_t *tmp = (seq_t *)realloc(cache,
                                       cache_cap * sizeof(seq_t));
        cache = tmp;
    }
}

/* ------------------------------------------------------------ */
/* Main sequence function                                         */
seq_t a(idx_t n)
{
    /* The sequence is 1‑based; for n==0 we return 0. */
    if (n == 0)
        return 0;

    /* Generate terms until the cache contains at least n entries. */
    while (cache_len < n) {
        ++last_checked;                       /* candidate integer */

        if (has05((seq_t)last_checked) &&
            !has05((seq_t)(last_checked + 1))) {
            ensure_cache_capacity();
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
