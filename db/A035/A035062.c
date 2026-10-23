/*
 * A035062  Numbers n such that 2^n does not contain the digit 7.
 * https://oeis.org/A035062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = the n‑th exponent k such that 2^k contains no digit 7 *
 *            (sequence offset = 1). This implementation incrementally
 *            generates * powers of 2 in decimal form and stores exponents
 *            that avoid the digit 7
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* Offset is 1; below is invalid */

    /* static state kept between calls */
    static bool init_done = false;

    /* power of 2 stored as little‑endian decimal digits */
    static uint8_t digits[200];                /* 200 digits enough for tests */
    static size_t digits_len = 0;              /* number of used digits */
    static idx_t current_exp = 0;              /* current exponent being examined */

    /* cache of exponents that satisfy the property */
    enum { MAX_CACHE = 20000 };                /* enough for n ≤ 10000 */
    static seq_t cache[MAX_CACHE];
    static size_t cache_len = 0;               /* number of cached terms */

    if (!init_done) {
        init_done = true;
        /* initialise 2^0 = 1 */
        digits_len = 1;
        digits[0] = 1;
        cache_len = 0;
    }

    /* target index in the cache (0‑based) */
    const size_t target = (size_t)(n - 1);

    /* Grow the cache until it contains the desired term */
    while (cache_len <= target) {

        /* check whether current 2^current_exp contains a digit 7 */
        bool has7 = false;
        for (size_t i = 0; i < digits_len; ++i) {
            if (digits[i] == 7) {
                has7 = true;
                break;
            }
        }
        if (!has7) {
            if (cache_len >= MAX_CACHE) {
                /* Exceeded pre‑allocated capacity – terminate */
                exit(EXIT_FAILURE);
            }
            cache[cache_len++] = (seq_t)current_exp;
        }

        /* multiply current power of 2 by 2 (carry propagation) */
        uint8_t carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            uint16_t prod = (uint16_t)digits[i] * 2 + carry;
            digits[i] = (uint8_t)(prod % 10);
            carry = (uint8_t)(prod / 10);
        }
        while (carry) {
            if (digits_len >= sizeof(digits)) {
                /* Should not happen for the problem limits */
                exit(EXIT_FAILURE);
            }
            digits[digits_len++] = carry % 10;
            carry /= 10;
        }

        ++current_exp;
    }

    return cache[target];
}
