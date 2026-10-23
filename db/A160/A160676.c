/*
 * A160676  Numbers k such that k and 2k use the same number of characters
 *          when expressed in Roman numerals.
 * https://oeis.org/A160676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 503, u32 n <= 503, u64 n <= 503,
 *            u128 n <= 503, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generates numbers k where Roman length(k) == Roman length(2k) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* roman_len_u64 returns the number of characters in the Roman numeral
   representation of the unsigned integer x.  */
static const uint64_t roman_values[] = {
    1000, 900, 500, 400, 100, 90,
    50, 40, 10, 9, 5, 4, 1
};
static const int roman_len_vals[] = {
    1, 2, 1, 2, 1, 2,
    1, 2, 1, 2, 1, 2, 1
};

static inline uint64_t roman_len_u64(uint64_t x)
{
    uint64_t len = 0;
    for (int i = 0; i < 13; ++i) {
        while (x >= roman_values[i]) {
            x -= roman_values[i];
            len += roman_len_vals[i];
        }
    }
    return len;
}

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *list = NULL;
    static size_t list_len = 0;
    static size_t list_cap = 0;
    static seq_t next_k = 1;   /* numbers to test, starting at 1 */

    /* extend the list until it contains at least n elements */
    while (list_len < n) {
        uint64_t k = (uint64_t)next_k;
        uint64_t len_k = roman_len_u64(k);
        uint64_t len_2k = roman_len_u64(k * 2);

        if (len_k == len_2k) {
            /* grow the array if necessary */
            if (list_len == list_cap) {
                list_cap = (list_cap == 0) ? 128 : list_cap * 2;
                list = (seq_t *)realloc(list, list_cap * sizeof(seq_t));
            }
            list[list_len++] = next_k;
        }

        /* move to the next candidate */
        next_k = (seq_t)(k + 1);
    }

    return list[n - 1];
}
