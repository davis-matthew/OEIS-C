/*
 * A089718  a(0) = 1, a(n) = number obtained by multiplying each digit of
 *          a(n-1) by n. May be called digitfactorial of n.
 * https://oeis.org/A089718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(0) = 1; a(n) = concatenate (digit * n) over the decimal digits
 *            of a(n‑1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   digit_len, pow10, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* memoised terms                */
    static size_t cap = 0;          /* current cache capacity       */
    static idx_t max_computed = (idx_t)-1;/* last index computed */

    /* Initialise cache for the first call */
    if (cache == NULL) {
        cache = seq_alloc(1);
        cap   = 1;
        cache[0] = (seq_t)1;
        max_computed = 0;
    }

    /* Return a cached value if already computed */
    if (n <= max_computed)
        return cache[n];

    /* Grow the cache if necessary */
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        cache = seq_realloc(cache, cap, new_cap);
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];

        /* split prev into its decimal digits (MSB → LSB) */
        size_t len = (size_t)digit_len(prev);
        uint8_t *digits = (uint8_t*)malloc(len);
        seq_t tmp = prev;
        for (size_t k = 0; k < len; ++k) {
            digits[len - 1 - k] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }

        /* build the next term by concatenating products */
        seq_t next = (seq_t)0;
        for (size_t j = 0; j < len; ++j) {
            seq_t prod = (seq_t)digits[j] * (seq_t)i;
            size_t dlen = (size_t)digit_len(prod);
            seq_t mult = pow10((idx_t)dlen);
            next = next * mult + prod;
        }

        free(digits);
        cache[i] = next;
    }

    max_computed = n;
    return cache[n];
}
