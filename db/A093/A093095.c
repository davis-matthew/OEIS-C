/*
 * A093095  Start with a(1) = 2, a(2) = 3, then apply the rule of A093094.
 * https://oeis.org/A093095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A093095:  a(1)=2, a(2)=3.
   For each consecutive pair (a(k),a(k+1)) compute p=a(k)*a(k+1)
   and append the decimal digits of p (most significant first)
   to the sequence.  This procedure generates all subsequent terms. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;      /* array of computed terms */
    static size_t capacity = 0;      /* size of allocated array */
    static size_t length = 0;        /* number of terms stored in cache */
    static idx_t pair_index = 0;     /* first index of the active pair */

    /* one‑time initialization */
    if (length == 0) {
        capacity = 4;                          /* start with room for a few terms */
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failed */
        cache[0] = (seq_t)2;                  /* a(1) */
        cache[1] = (seq_t)3;                  /* a(2) */
        length   = 2;
        pair_index = 0;                       /* start with pair (a(1),a(2)) */
    }

    /* Produce terms until we have at least n of them */
    while (length < (size_t)n) {
        /* Compute product of the current consecutive pair */
        seq_t prod = cache[pair_index] * cache[pair_index + 1];

        /* Extract decimal digits of prod (least‑significant first) */
        unsigned char digits[32];
        size_t digit_cnt = 0;
        seq_t tmp = prod;
        if (tmp == (seq_t)0) {
            digits[0] = 0;
            digit_cnt = 1;
        } else {
            while (tmp != (seq_t)0) {
                digits[digit_cnt++] = (unsigned char)(tmp % (seq_t)10);
                tmp /= (seq_t)10;
            }
        }

        /* Ensure enough capacity for the new digits */
        if (length + digit_cnt > capacity) {
            size_t new_cap = capacity ? capacity << 1 : 4;
            while (new_cap < length + digit_cnt) new_cap <<= 1;
            seq_t *tmp_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp_ptr) return (seq_t)0;    /* allocation failed */
            cache = tmp_ptr;
            capacity = new_cap;
        }

        /* Append digits from most‑significant to least‑significant */
        for (size_t i = 0; i < digit_cnt; ++i) {
            cache[length++] = (seq_t)digits[digit_cnt - 1 - i];
        }

        /* Move to next pair (a(k+1), a(k+2)) for next iteration */
        ++pair_index;
    }

    return cache[(size_t)n - 1];
}
