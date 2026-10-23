/*
 * A033065  Numbers whose base-14 representation Sum_{i=0..m} d(i)*14^i has
 *          odd d(m-i) for all odd i.
 * https://oeis.org/A033065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n-th positive integer whose base-14 representation
    has odd digits in the positions m-i for every odd i (where 
    m is the highest digit index).  The sequence is found by
    brute‑force enumeration.                                            */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool is_valid(seq_t x)
{
    /* Gather the base‑14 digits of x (least significant first). */
    seq_t tmp = x;
    size_t len = 0;
    size_t cap = 8;
    uint8_t *digits = (uint8_t *)malloc(cap);
    if (!digits) return false;          /* memory failure should not occur */

    /* Extract digits.   */
    while (tmp > 0) {
        if (len == cap) {
            cap <<= 1;
            uint8_t *nb = (uint8_t *)realloc(digits, cap);
            if (!nb) { free(digits); return false; }
            digits = nb;
        }
        /* remainder 0 <= r < 14 */
        uint64_t r = (uint64_t)(tmp % (seq_t)14);
        digits[len++] = (uint8_t)r;
        tmp = tmp / (seq_t)14;
    }

    if (len == 0) {            /* x == 0 is not considered a term */
        free(digits);
        return false;
    }

    size_t m = len - 1;        /* most significant digit index */

    /* Check the property: for every odd i, digit d(m-i) must be odd. */
    for (size_t i = 0; i < len; ++i) {
        if (((m - i) & 1) == 1) {          /* (m-i) is odd */
            if ((digits[i] & 1) == 0) {    /* digit is even */
                free(digits);
                return false;
            }
        }
    }

    free(digits);
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cand = 1;              /* start checking from 1 */

    while (1) {
        if (is_valid(cand)) {
            ++count;
            if (count == n) return cand;
        }
        ++cand;
    }
}
