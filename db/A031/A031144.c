/*
 * A031144  Numbers n such that n! has a record number of zeros.
 * https://oeis.org/A031144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 476, u32 n <= 476, u64 n <= 476,
 *            u128 n <= 476, big any n
 * Helpers:   ensure_records_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: record indices of factorials whose decimal
 * representation has a record high number of zero digits.
 * This is a brute‑force traversal of n! using a simple decimal
 * big integer.  Recorded indices are cached in a static array so
 * that any order of calls works correctly. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_records_cap.h"

/* helper: extend an array of digits if needed */
static void ensure_digits_cap(size_t required_cap, uint8_t **digits, size_t *cap) {
    if (required_cap <= *cap) return;
    size_t new_cap = (*cap) ? *cap * 2 : 16;
    while (new_cap < required_cap) new_cap *= 2;
    uint8_t *tmp = (uint8_t *)realloc(*digits, new_cap * sizeof(uint8_t));
    if (!tmp) { free(*digits); *digits = NULL; *cap = 0; return; }
    *digits = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined but return 0 */

    /* static state */
    static uint8_t  *digits      = NULL;   /* decimal digits, least
                                            significant first */
    static size_t   digits_len = 0, digits_cap = 0;
    static idx_t    last_n = 0;           /* current factorial index */

    static idx_t   *records      = NULL;
    static size_t  rec_size = 0, rec_cap = 0;
    static int     max_zeros = 0;         /* highest zero count seen */

    /* initialization on first call */
    if (!rec_size) {
        /* factorial 0! = 1 */
        ensure_digits_cap(1, &digits, &digits_cap);
        digits[0] = 1;
        digits_len = 1;
        last_n = 0;

        /* record the first index 0 */
        ensure_records_cap(1, &records, &rec_cap);
        records[0] = 0;
        rec_size = 1;
        max_zeros = 0;
    }

    /* compute until we have at least n record indices */
    while (rec_size < (size_t)n) {
        idx_t next_n = last_n + 1;

        /* multiply current factorial by next_n */
        uint32_t carry = 0;
        uint32_t mult  = (uint32_t)next_n;
        for (size_t i = 0; i < digits_len; ++i) {
            uint32_t prod = (uint32_t)digits[i] * mult + carry;
            digits[i] = (uint8_t)(prod % 10);
            carry = prod / 10;
        }
        while (carry) {
            ensure_digits_cap(digits_len + 1, &digits, &digits_cap);
            digits[digits_len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }

        /* count zero digits */
        int zeros = 0;
        for (size_t i = 0; i < digits_len; ++i)
            if (digits[i] == 0) ++zeros;

        if (zeros > max_zeros) {
            /* new record */
            ensure_records_cap(rec_size + 1, &records, &rec_cap);
            records[rec_size++] = next_n;
            max_zeros = zeros;
        }

        last_n = next_n;
    }

    return (seq_t)records[n - 1];
}
