/*
 * A157911  Nonprimes whose digits are all cubes.
 * https://oeis.org/A157911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   enumerate all non‑prime numbers whose digits are 0, 1 or 8, * in
 *            increasing order. The first terms are 0, 1, 8, 10, 18, … (see
 *            OEIS A157911)
 * Range:     u8 n <= 15, u16 n <= 146, u32 n <= 187, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */

/* Cache of the non‑prime cube‑digit numbers found so far. */
static seq_t *cache      = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;
static uint8_t next_len   = 1;   /* length of the numbers to generate next */

/* Recursively generate all numbers of a given length 'len' using
 * the digits 0, 1, 8. When a full length number is reached it is
 * tested for primality; if it is not prime it is stored in the cache. */
static void generate_len(uint8_t len, seq_t curr, uint8_t pos)
{
    static const uint8_t digits[3] = {0, 1, 8};

    if (pos == len) {
        /* Store the number if it is not prime. */
        if (!is_prime_big(curr)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* out of memory */
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = curr;
        }
        return;
    }

    for (unsigned i = 0; i < 3; ++i) {
        uint8_t d = digits[i];
        /* Skip a leading zero for numbers longer than one digit. */
        if (pos == 0 && len > 1 && d == 0)
            continue;
        generate_len(len, curr * 10 + d, pos + 1);
    }
}

/* Generate terms until the cache contains at least 'need' items. */
static void ensure_terms(idx_t need)
{
    while (cache_len < (size_t)need) {
        generate_len(next_len, 0, 0);
        ++next_len;
    }
}

/* Return the nth term (1‑based) of OEIS A157911. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n < 1, safe guard */
    ensure_terms(n);
    return cache[(size_t)n - 1];
}
