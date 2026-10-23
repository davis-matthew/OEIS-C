/*
 * A127109  n! in base 5.
 * https://oeis.org/A127109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the representation of n! in base 5, read as a base‑10
 *            integer
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For example: 3! = 6 = 11₂ base 5 ⇒ a(3) = 11. */
#include <stdint.h>
#include <stdlib.h>

#define BASE_DIGITS 11
/* 5^11 = 48828125 */
#define BASE ((uint32_t)48828125U)

seq_t a(idx_t n)
{
    /* Static cache to store n! in base‑5, packed in words of BASE */
    static uint32_t *words = NULL;   /* least‑significant word first */
    static size_t words_cap = 0;     /* allocated capacity (words) */
    static size_t words_len = 0;     /* number of used words */
    static idx_t     cached_n  = 0;  /* last n computed */

    /* Initialise (or reset) the cache for 0! */
    if (words == NULL || n == 0) {
        if (words_cap == 0) {
            words_cap = 16;
            words = (uint32_t *)malloc(words_cap * sizeof(uint32_t));
            if (!words) abort();
        }
        words_len = 1;
        words[0] = 1U;   /* 0! = 1 */
        cached_n  = 0;
    } else if (n < cached_n) {
        /* Requested smaller n than cached – reset to 0! */
        words_len = 1;
        words[0] = 1U;
        cached_n = 0;
    }

    /* Multiply incrementally up to n */
    while (cached_n < n) {
        ++cached_n;               /* multiply by cached_n (1 … n) */
        uint64_t carry = 0;
        for (size_t i = 0; i < words_len; ++i) {
            uint64_t prod = (uint64_t)words[i] * cached_n + carry;
            words[i] = (uint32_t)(prod % BASE);
            carry    = prod / BASE;
        }
        while (carry > 0) {
            if (words_len == words_cap) {
                size_t new_cap = words_cap ? words_cap << 1 : 16;
                uint32_t *tmp = (uint32_t *)realloc(words, new_cap * sizeof(uint32_t));
                if (!tmp) { free(words); abort(); }
                words = tmp;
                words_cap = new_cap;
            }
            words[words_len++] = (uint32_t)(carry % BASE);
            carry /= BASE;
        }
    }

    /* Convert the base‑5 representation to its decimal‑digit string */
    size_t max_digits = words_len * BASE_DIGITS;   /* safe upper bound */
    uint8_t *digits_lsd = (uint8_t *)malloc(max_digits);
    if (!digits_lsd) abort();

    size_t digits_len = 0;
    for (size_t i = 0; i < words_len; ++i) {
        uint32_t w = words[i];
        for (int k = 0; k < BASE_DIGITS; ++k) {
            digits_lsd[digits_len++] = (uint8_t)(w % 5);
            w /= 5;
        }
    }

    /* Trim leading zeros (trailing zeros in LSD order) */
    while (digits_len > 1 && digits_lsd[digits_len - 1] == 0) {
        --digits_len;
    }

    /* Build the result as a decimal integer from the digit string */
    seq_t result = 0;
    for (size_t i = digits_len; i > 0; --i) {
        result = result * 10 + digits_lsd[i - 1];
    }

    free(digits_lsd);
    return result;
}
