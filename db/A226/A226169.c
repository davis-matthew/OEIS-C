/*
 * A226169  Niven numbers when expressed in bases 1 through 10.
 * https://oeis.org/A226169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 314, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9841 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Niven numbers in bases 1 through 10: a(n) is the n‑th integer that is
   divisible by the sum of its digits in every base 2,3,…,10. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Increment a number's representation in base `b` and update its digit sum. */
static void incr_base(int b,
                      uint8_t *digits,    /* digits[b][...], array of length MAX_DIGITS */
                      uint16_t *sum,      /* current digit sum */
                      uint8_t *len)       /* current number of digits */
{
    size_t i = 0;
    bool carry = true;
    while (carry && i < *len) {
        uint8_t d = digits[i];
        if (d + 1 == (uint8_t)b) {
            digits[i] = 0;
            *sum -= (b - 1);
            i++;
            /* carry stays true */
        } else {
            digits[i] = d + 1;
            *sum += 1;
            carry = false;
        }
    }
    if (carry) {                     /* added a new most significant digit */
        digits[*len] = 1;
        *sum += 1;
        (*len)++;
    }
}

#define MAX_BASE   10          /* highest base used */
#define MAX_DIGITS 200         /* enough for all expected values */

/* Cache of found terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0, cache_sz = 0;

/* Ensure cache has room for at least `needed` elements. */
static void ensure_cap(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed <= *cap) return;
    size_t newcap = (*cap == 0) ? 4 : (*cap << 1);
    if (newcap < needed) newcap = needed;
    seq_t *tmp = mem_resize(seq_t, *arr, newcap);
    if (!tmp) exit(1);                /* allocation failed */
    *arr = tmp;
    *cap = newcap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe */

    static bool initialized = false;
    static seq_t cur = 0;            /* current candidate number */

    /* Per-base digit arrays and sums (static to avoid reallocation) */
    static uint8_t  digits[MAX_BASE + 1][MAX_DIGITS];
    static uint16_t sum_digits[MAX_BASE + 1];
    static uint8_t  len_digits[MAX_BASE + 1];

    if (!initialized) {
        initialized = true;
        /* initialise per-base data */
        for (int b = 2; b <= MAX_BASE; ++b) {
            sum_digits[b]   = 0;
            len_digits[b]   = 0;
            for (int i = 0; i < MAX_DIGITS; ++i)
                digits[b][i] = 0;
        }
    }

    /* Generate terms until we have at least n of them cached */
    while (cache_sz < (size_t)n) {
        /* next candidate */
        cur += (seq_t)1;

        /* update digit sums for all bases */
        for (int b = 2; b <= MAX_BASE; ++b)
            incr_base(b, digits[b], &sum_digits[b], &len_digits[b]);

        /* check divisibility conditions */
        bool good = true;
        for (int b = 2; b <= MAX_BASE; ++b) {
            seq_t s = (seq_t)sum_digits[b];
            if (cur % s != 0) { good = false; break; }
        }

        if (good) {
            ensure_cap(cache_sz + 1, &cache_cap, &cache);
            cache[cache_sz++] = cur;
        }
    }

    return cache[n - 1];
}
