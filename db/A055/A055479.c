/*
 * A055479  Powers of ten written in base 9.
 * https://oeis.org/A055479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = decimal interpretation of base‑9 digits of 10^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state for incremental computation */
    static seq_t *cache = NULL;               /* cached a values      */
    static size_t cache_cap = 0;              /* current cache size   */
    static idx_t max_n = (idx_t)-1;           /* highest n computed   */
    static uint8_t *digits = NULL;            /* base‑9 digits of 10^n */
    static size_t digits_cap = 0;             /* capacity of digits   */
    static size_t digits_len = 0;             /* actual length        */
    static bool initialized = false;          /* init flag            */

    if (!initialized) {
        /* initialise for n = 0  -> 10^0 = 1  (base‑9: "1") */
        max_n = 0;
        digits_cap = 16;
        digits = (uint8_t*)malloc(digits_cap);
        digits[0] = 1;
        digits_len = 1;

        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;

        initialized = true;
    }

    if (n <= max_n) {
        /* already computed */
        return cache[n];
    }

    /* enlarge cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1)) new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute a(n) for all intermediate n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* multiply current digits by 10 decimal in base‑9 */
        unsigned int carry = 0;
        for (size_t j = 0; j < digits_len; ++j) {
            unsigned int prod = (unsigned int)digits[j] * 10 + carry;
            digits[j] = prod % 9;
            carry = prod / 9;
        }
        while (carry > 0) {
            if (digits_len == digits_cap) {
                digits_cap *= 2;
                digits = (uint8_t*)realloc(digits, digits_cap);
            }
            digits[digits_len++] = carry % 9;
            carry /= 9;
        }

        /* convert base‑9 digits to the required decimal value */
        seq_t val = (seq_t)0;
        for (size_t j = digits_len; j > 0; --j) {
            val = val * (seq_t)10 + (seq_t)digits[j - 1];
        }
        cache[i] = val;
    }

    max_n = n;
    return cache[n];
}
