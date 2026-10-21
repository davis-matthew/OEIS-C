/*
 * A076395  Squares whose external digits form a square and whose internal
 *          digits form a prime.
 * https://oeis.org/A076395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, less, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A076395: squares whose external digits form a square and whose internal digits form a prime.
   Computed by a brute‑force search over squares. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_m = 1;

static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    while (cache_len < (size_t)n) {
        seq_t s = (seq_t)next_m * (seq_t)next_m;
        int len = digit_len(s);
        if (len >= 3) {
            seq_t pow10len_minus1 = pow10((idx_t)(len - 1));
            seq_t first_digit = s / pow10len_minus1;
            seq_t last_digit = s % 10;
            uint64_t ext = (uint64_t)first_digit * 10u + (uint64_t)last_digit;
            if (ext == 16u || ext == 25u || ext == 36u ||
                ext == 49u || ext == 64u || ext == 81u) {
                seq_t internal = (s % pow10len_minus1) / 10;
                if (internal > 1 && is_prime_big(internal)) {
                    ensure_cache_capacity(cache_len + 1);
                    cache[cache_len++] = s;
                }
            }
        }
        ++next_m;
    }
    return cache[n - 1];
}
