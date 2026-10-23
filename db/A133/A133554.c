/*
 * A133554  Lesser of emirps (pairs) with only prime digits (A128388).
 * https://oeis.org/A133554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the lesser of the emirp pairs with only prime digits
 *            (A128388)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 7, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   append_candidate, is_prime_big, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/append_candidate.h"
#include "../utils/is_prime_big.h"
#include "../utils/rev_decimal.h"

/* Cached terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;

/* Current length being examined */
static idx_t current_len = 1;

/* Prime digits used to construct candidates */
static const char prime_digits[4] = {'2', '3', '5', '7'};

/* Recursive generator of numbers of a given length */
static void rec_gen(idx_t len, char *digits, idx_t pos)
{
    if (pos == len) {
        /* Convert digit array to a number */
        seq_t n = 0;
        for (idx_t i = 0; i < len; ++i)
            n = n * 10 + (seq_t)(digits[i] - '0');

        seq_t rev = rev_decimal(n);
        if (rev == n)   /* skip palindromes */
            return;

        if (is_prime_big(n) && is_prime_big(rev)) {
            if (n < rev)
                append_candidate(&cache, &cache_size, &cache_cap, n);
        }
        return;
    }

    for (int k = 0; k < 4; ++k) {
        digits[pos] = prime_digits[k];
        rec_gen(len, digits, pos + 1);
    }
}

/* Generate all numbers of a specific length and collect emirps */
static void generate_len(idx_t len)
{
    char digits[32];            /* enough for all practical lengths */
    rec_gen(len, digits, 0);
}

/* Public function for sequence A133554 */
seq_t a(idx_t n)
{
    /* Lazily compute terms until we have at least n of them */
    while (cache_size < n) {
        ++current_len;
        generate_len(current_len);
    }
    return cache[n - 1];   /* offset 1: a(1) is the first term */
}
