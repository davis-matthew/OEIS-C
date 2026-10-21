/*
 * A210584  List of Lyndon words on {1,2,3,4} sorted first by length and then
 *          lexicographically.
 * https://oeis.org/A210584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 294, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_lyndon
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth Lyndon word over the alphabet {1,2,3,4},
 * sorted first by length and then lexicographically.
 * The returned value is the word written as a decimal integer,
 * e.g. the word 1 4 2 is returned as 142.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_lyndon.h"

seq_t a(idx_t n)
{
    /* static cache of generated Lyndon words */
    static seq_t *words = NULL;
    static size_t words_count = 0;
    static size_t words_cap = 0;
    static size_t next_len = 1;   /* next word length to generate */

    if (n == 0) return 0;   /* undefined but safe */

    /* generate words until we have at least n entries */
    while (words_count < (size_t)n) {
        size_t L = next_len;

        /* total candidates for this length: 4^L */
        uint64_t total = 1;
        for (size_t i = 0; i < L; ++i) total *= 4;

        for (uint64_t num = 0; num < total; ++num) {
            /* convert num to base‑4 digits (0‑3), then add 1 to get 1‑4 */
            unsigned char digits[32];
            uint64_t tmp = num;
            for (int pos = (int)L - 1; pos >= 0; --pos) {
                digits[pos] = (unsigned char)((tmp % 4) + 1);
                tmp /= 4;
            }

            if (!is_lyndon(digits, L)) continue;

            /* encode as decimal number */
            seq_t value = 0;
            for (size_t k = 0; k < L; ++k)
                value = value * 10 + (seq_t)digits[k];

            /* ensure capacity of cache */
            if (words_count >= words_cap) {
                size_t new_cap = words_cap ? words_cap * 2 : 8;
                seq_t *tmp_arr = (seq_t *)realloc(words, new_cap * sizeof(seq_t));
                if (!tmp_arr) exit(1); /* abort on allocation failure */
                words = tmp_arr;
                words_cap = new_cap;
            }
            words[words_count++] = value;
        }
        next_len = L + 1;
    }

    return words[n - 1];
}
