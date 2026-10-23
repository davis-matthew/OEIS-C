/*
 * A032732  Numbers k such that k prefixed by '7' and followed by '9' is
 *          prime.
 * https://oeis.org/A032732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 29, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032732: a(n) is the n‑th integer k such that 7 k 9 is prime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_k = 0;          /* next k to test */
static idx_t next_threshold = 10; /* 10^digits(k)   */
static seq_t pow10 = 100;         /* 10^(digits(k)+1) */

/* Ensure the cache can store at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, 0 for safety */

    /* Generate terms until the cache holds at least n entries */
    while (cache_len < (size_t)n)
    {
        /* Adjust digit length when k reaches the next threshold 10^d */
        if (next_k == next_threshold) {
            next_threshold *= 10;      /* 10^d → 10^(d+1) */
            pow10      *= 10;          /* 10^(d+1) → 10^(d+2) */
        }

        /* Candidate number: 7 * 10^(digits+1) + k * 10 + 9 */
        seq_t candidate = 7 * pow10;
        candidate += ((seq_t)next_k) * 10;
        candidate += 9;

        if (is_prime_big(candidate)) {
            grow_cache(cache_len + 1);
            cache[cache_len] = next_k;
            ++cache_len;
        }
        ++next_k;
    }

    return cache[n - 1];
}
