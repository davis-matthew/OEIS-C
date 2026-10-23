/*
 * A182180  Semiprimes that become prime when their digits are sorted into
 *          nonincreasing order.
 * https://oeis.org/A182180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth semiprime that becomes prime when its decimal
 *            digits are sorted in non‑increasing order
 * Range:     u8 n <= 5, u16 n <= 467, u32 n <= 467, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_prime_factors, digit_len, is_prime_big, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/count_prime_factors.h"
#include "../utils/seq_to_str.h"
#include "../utils/digit_len.h"

/* Cache for already found terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/* Next integer to test */
static seq_t next_candidate = 2;

/* Append a value to the cache, growing it if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap ? cache_cap * 2 : 16);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Convert a number to a string and return a copy of the digits
   sorted in non‑increasing order as a new number. */
static seq_t sort_digits_desc(seq_t x)
{
    /* Allocate a buffer large enough for the decimal representation. */
    size_t buf_len = (size_t)digit_len(x) + 1;
    char *buf = (char *)malloc(buf_len);
    if (!buf) return 0; /* should not happen */

    size_t len = 0;
    seq_to_str(x, buf, &len);          /* buf now contains the decimal string */
    /* Convert to an array of digits. */
    uint8_t *dig = (uint8_t *)malloc(len);
    if (!dig) { free(buf); return 0; }

    for (size_t i = 0; i < len; ++i)
        dig[i] = (uint8_t)(buf[i] - '0');

    /* Simple insertion sort in descending order. */
    for (size_t i = 1; i < len; ++i) {
        uint8_t key = dig[i];
        size_t j = i;
        while (j > 0 && dig[j - 1] < key) {
            dig[j] = dig[j - 1];
            --j;
        }
        dig[j] = key;
    }

    /* Build the sorted number from the digit array. */
    seq_t res = 0;
    for (size_t i = 0; i < len; ++i)
        res = res * 10 + dig[i];

    free(dig);
    free(buf);
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1 */

    /* Generate terms until the cache holds at least n terms. */
    while (cache_len < n) {
        /* Test current candidate. */
        if (count_prime_factors(next_candidate) == (seq_t)2) {
            seq_t sorted = sort_digits_desc(next_candidate);
            if (is_prime_big(sorted))
                cache_append(next_candidate);
        }
        /* Increment to the next integer. */
        ++next_candidate;
    }

    return cache[n - 1];
}
