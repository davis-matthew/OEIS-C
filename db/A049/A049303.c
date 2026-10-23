/*
 * A049303  Numbers k such that k is a substring of 5^k.
 * https://oeis.org/A049303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth integer k such that the decimal representation
 *            of 5^k contains the decimal representation of k. The function
 *            searches for k incrementally, maintaining 5^k in a decimal digit
 *            array
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state */
    static bool initialized = false;
    static idx_t current_k = 1;          /* current exponent k       */
    static size_t digits_len = 0;        /* number of decimal digits */
    static uint8_t *digits = NULL;       /* little‑endian decimal     */
    static seq_t *cache = NULL;          /* stored terms             */
    static size_t cache_size = 0;        /* cached element count     */
    static size_t cache_cap = 0;         /* cache capacity           */

    if (!initialized) {
        /* 5^1 = 5 */
        digits_len = 1;
        digits = (uint8_t *)malloc(digits_len * sizeof(uint8_t));
        digits[0] = 5;

        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
        initialized = true;
    }

    /* generate terms until we have the requested number */
    while (cache_size < (size_t)n) {
        /* build decimal string of 5^current_k (MSB first) */
        char *num_str = (char *)malloc(digits_len + 1);
        for (size_t i = 0; i < digits_len; ++i) {
            num_str[digits_len - 1 - i] = '0' + digits[i];
        }
        num_str[digits_len] = '\0';

        /* convert current_k to string */
        char k_str[32];
        sprintf(k_str, "%llu", (unsigned long long)current_k);

        /* test for substring */
        if (strstr(num_str, k_str) != NULL) {
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)current_k;
        }
        free(num_str);

        /* advance to next exponent */
        ++current_k;

        /* multiply digits by 5 */
        uint32_t carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            uint32_t prod = (uint32_t)digits[i] * 5 + carry;
            digits[i] = (uint8_t)(prod % 10);
            carry = prod / 10;
        }
        while (carry) {
            digits = (uint8_t *)realloc(digits, (digits_len + 1) * sizeof(uint8_t));
            digits[digits_len] = (uint8_t)(carry % 10);
            carry /= 10;
            ++digits_len;
        }
    }

    return cache[n - 1];
}
