/*
 * A213310  Numbers with exactly 3 nonprime substrings (substrings with
 *          leading zeros are considered to be nonprime).
 * https://oeis.org/A213310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 310, u64 n <= 310, u128 n <= 310,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): Numbers with exactly 3 nonprime substrings
   (search / brute‑force generation up to 100 000) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* Build the table of values once. */
static void init_cache(void)
{
    if (cache != NULL)
        return;                       /* already built */

    const size_t LIMIT = 100000;           /* numbers below this satisfy the bound */
    const size_t CAP0  = 512;              /* initial capacity */

    /* Dynamic array for results */
    cache_cap = CAP0;
    cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));

    /* Sieve of Eratosthenes up to LIMIT */
    bool *prime = (bool*)calloc(LIMIT + 1, sizeof(bool));
    for (size_t i = 2; i <= LIMIT; ++i) prime[i] = true;
    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (prime[p]) {
            for (size_t q = p * p; q <= LIMIT; q += p)
                prime[q] = false;
        }
    }

    /* Temporary buffer for digits (max 6 digits for 100 000) */
    uint8_t digits[6];
    size_t len;

    /* Generate all numbers from 10 to 99 999 */
    for (seq_t num = 10; num <= 99999; ++num) {
        /* Extract digits into digits[0..len-1] (MSB first) */
        seq_t temp = num;
        len = 0;
        do {
            digits[len++] = (uint8_t)(temp % 10);
            temp /= 10;
        } while (temp);
        for (size_t i = 0, j = len - 1; i < j; ++i, --j) {
            uint8_t t = digits[i];
            digits[i] = digits[j];
            digits[j] = t;
        }

        /* Count nonprime substrings */
        int nonprime_cnt = 0;
        for (size_t i = 0; i < len; ++i) {
            seq_t val = 0;
            for (size_t j = i; j < len; ++j) {
                val = val * 10 + digits[j];
                if (digits[i] == 0) {
                    /* Any substring starting with '0' is nonprime */
                    ++nonprime_cnt;
                } else {
                    if (!prime[(size_t)val]) {
                        ++nonprime_cnt;
                    }
                }
            }
        }

        if (nonprime_cnt == 3) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = num;
        }
    }
    free(prime);
}

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* index below offset: undefined */

    init_cache();

    if ((idx_t)n <= cache_len)
        return cache[n - 1];

    return 0;                         /* out of range, sequence is finite */
}
