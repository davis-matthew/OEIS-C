/*
 * A064827  Numbers k such that each digit of k occurs among the digits of
 *          k^2.
 * https://oeis.org/A064827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_digits, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search: for each positive integer k,
   test whether every occurrence of a decimal digit in k is also
   present in the decimal representation of k².  The n‑th term a(n)
   is returned on a 1‑based index. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_digits.h"
#include "../utils/seq_mul.h"

/* Cache of found terms. */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_k = 1;   /* next integer to test */

/* Ensure the cache array can hold at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);              /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Count decimal digits of v into freq[10]. */
static void digit_frequencies(seq_t v, unsigned short freq[10])
{
    uint8_t buf[256];
    size_t len, i;
    for (i = 0; i < 10; ++i) freq[i] = 0;
    get_digits(v, buf, &len);
    for (i = 0; i < len; ++i)
        ++freq[buf[i]];
}

/* Return true iff every occurrence of each decimal digit in k
   appears at least as many times in k². */
static bool k_is_good(seq_t k)
{
    unsigned short freq_k[10], freq_sq[10];
    digit_frequencies(k, freq_k);

    seq_t sq = seq_mul(k, k);   /* k² */
    digit_frequencies(sq, freq_sq);

    for (int d = 0; d < 10; ++d)
        if (freq_k[d] > freq_sq[d])
            return false;
    return true;
}

/* Return the n‑th term of A064827 (1‑based).  The function is
   order‑independent and memoises its results. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* undefined for 0 or negative indices */

    /* Expand the cache until it contains term n. */
    while (cache_len < (size_t)n) {
        if (k_is_good(next_k)) {      /* only store valid terms */
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = next_k;
        }
        ++next_k;                      /* always advance the candidate */
    }
    return cache[(size_t)(n - 1)];
}
