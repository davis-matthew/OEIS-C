/*
 * A023080  Positive numbers k such that k and 3*k are anagrams in base 9
 *          (written in base 9).
 * https://oeis.org/A023080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 164, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerates positive integers k such that k and 3*k are
   anagrams in base 9 and returns the base‑9 digits of k interpreted as a
   decimal integer.  The sequence is indexed the OEIS way (a(1) is the
   first term). */

/*======================================================================*/

#include <stdlib.h>
#include <stdint.h>

/* Helper: count how many seq_t terms are needed in the cache */
static seq_t *
ensure_cache(size_t needed, seq_t **cache, size_t *size, size_t *cap)
{
    if (*cap == 0) {
        *cap = 128;
        *cache = (seq_t *)malloc(*cap * sizeof(seq_t));
    } else if (*size == *cap) {
        size_t new_cap = *cap * 2;
        *cache = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
    return *cache;
}

/*======================================================================*/

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    static uint64_t next_k = 1;          /* candidate k (decimal) */

    /* generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        uint64_t k = next_k++;           /* decimal integer k */

        /* convert k to base‑9 digits (least significant first) */
        uint8_t digits[32];
        size_t len = 0;
        uint64_t tmp = k;
        do {
            digits[len++] = (uint8_t)(tmp % 9);
            tmp /= 9;
        } while (tmp > 0);

        /* multiply by 3 in base‑9, produce prod[] and a carry */
        uint8_t prod[32];
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint64_t v = (uint64_t)digits[i] * 3 + carry;
            prod[i] = (uint8_t)(v % 9);
            carry   = v / 9;
        }
        /* if the product has more digits, reject */
        if (carry != 0)
            continue;

        /* count digit frequencies of k and its product */
        uint8_t freq_k[9] = {0};
        uint8_t freq_p[9] = {0};
        for (size_t i = 0; i < len; ++i) {
            ++freq_k[digits[i]];
            ++freq_p[prod[i]];
        }

        /* compare frequencies */
        int ok = 1;
        for (int d = 0; d < 9; ++d)
            if (freq_k[d] != freq_p[d]) {
                ok = 0;
                break;
            }
        if (!ok)
            continue;

        /* convert the base‑9 digits of k to a decimal integer */
        seq_t value = 0;
        for (size_t i = len; i-- > 0; ) {
            value = value * 10 + digits[i];
        }

        /* store the term in the cache */
        cache = ensure_cache(cache_size + 1, &cache, &cache_size, &cache_cap);
        cache[cache_size++] = value;
    }

    return cache[n - 1];
}
