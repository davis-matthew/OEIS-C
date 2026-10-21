/*
 * A081463  Numbers which when multiplied by their final digit have products
 *          with same digital sequence except that last is first. Numbers
 *          obtained by concatenating a term any number of times with itself
 *          also have the defining property and are omitted.
 * https://oeis.org/A081463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: computes a term of OEIS A081463.
   For each final digit d the minimal integer x with
   x*d equal to the rotation of the digits of x is constructed
   by iterating the multiplication-by-d operation from the
   least significant digit upward. The sequence contains only
   the nine numbers corresponding to the digits 1..9.  */

/* Include standard headers for malloc/realloc and uint8_t. */
#include <stdlib.h>
#include <stdint.h>

/* Build a cache of the nine terms by computing them on demand. */
static seq_t cache[10] = {0};

seq_t a(idx_t n)
{
    /* Map index n to the final digit d that defines the term. */
    /* The mapping (n -> last digit) for the OEIS sequence is: */
    /* 1->1, 2->4, 3->8, 4->2, 5->7, 6->3, 7->5, 8->9, 9->6. */
    static const int d_map[10] = {0, 1, 4, 8, 2, 7, 3, 5, 9, 6};

    /* If the term has already been computed, return it. */
    if (n < 1) return 0;
    if (n <= 9 && cache[(int)n] != 0) return cache[(int)n];

    /* For n > 9 we simply cycle through the 9 available terms. */
    int idx = (int)((n - 1) % 9) + 1;
    int d = d_map[idx];

    /* Special case for d == 1: the number is simply 1. */
    if (d == 1) {
        seq_t res = 1;
        if (n <= 9) cache[idx] = res;
        return res;
    }

    /* Construct the digits from least significant to most significant. */
    size_t cap = 64;
    uint8_t *digits = (uint8_t*)malloc(cap);
    if (!digits) return 0;              /* OOM guard */
    size_t len = 1;
    digits[0] = (uint8_t)d;             /* units digit */
    uint8_t current = digits[0];
    uint8_t carry = 0;

    while (1) {
        seq_t prod = (seq_t)current * d + carry;
        uint8_t new_digit = (uint8_t)(prod % 10);
        carry = (uint8_t)(prod / 10);

        if (new_digit == d && carry == 0) break;

        if (len == cap) {
            cap <<= 1;
            uint8_t *tmp = (uint8_t*)realloc(digits, cap);
            if (!tmp) { free(digits); return 0; }
            digits = tmp;
        }
        digits[len++] = new_digit;
        current = new_digit;
    }

    /* Assemble the number from most significant to least. */
    seq_t result = 0;
    for (size_t i = len; i > 0; --i) {
        result = result * 10 + digits[i - 1];
    }

    free(digits);
    if (n <= 9) cache[idx] = result;
    return result;
}
