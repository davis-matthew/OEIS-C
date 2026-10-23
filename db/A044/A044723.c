/*
 * A044723  Numbers n such that string 1,0 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044723
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

/* Implementation: brute‑force search for the n‑th integer whose decimal
   representation contains the substring "10" but whose successor does not. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/* Return true if x contains the substring "10" in base 10. */
static bool has10(seq_t x)
{
    if (x < (seq_t)10) return false;          /* at least two digits needed */
    uint8_t digits[64];
    size_t dcnt = 0;
    seq_t tmp = x;
    while (tmp > (seq_t)0) {
        digits[dcnt++] = (uint8_t)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }
    for (size_t k = 1; k < dcnt; ++k) {
        if (digits[k] == 1 && digits[k - 1] == 0) return true;
    }
    return false;
}

/* External function required by the tests. */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t size   = 0;
    static size_t capacity = 0;
    static seq_t next_k = (seq_t)0;

    if (n == (idx_t)0) return (seq_t)0;  /* undefined for offset 0 */

    /* initialise the cache on the first call */
    if (!cache) {
        capacity = 256;                    /* arbitrary small start */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();               /* allocation failure */
    }

    /* grow the cache if needed */
    if (size >= capacity) {
        size_t new_cap = capacity * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = new_cap;
    }

    /* generate terms until we have at least n of them */
    while (size < (size_t)n) {
        /* find the next candidate */
        for (;;) {
            if (has10(next_k)) {
                seq_t kp1 = next_k + (seq_t)1;
                if (!has10(kp1)) break;
            }
            next_k = next_k + (seq_t)1;
        }
        /* store the term */
        cache[size++] = next_k;
        /* advance to the next number after the found term */
        next_k = next_k + (seq_t)1;
    }

    return cache[(size_t)n - 1];
}
