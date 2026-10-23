/*
 * A034293  Numbers k such that 2^k does not contain the digit 2 (probably
 *          finite).
 * https://oeis.org/A034293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A034293: a(n) = the n‑th exponent k such that 2^k contains no digit 2.
   The function keeps a memoized list of found exponents and enlarges it on demand.
   It uses a simple decimal big‑integer representation stored as an array of
   base‑10 digits (least significant first).  No external utilities are needed.  */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Return 0 for a(0); not part of the defined offset but harmless */
    if (n == 0) return (seq_t)0;

    /* Static state for memoization */
    static seq_t *terms    = NULL;   /* array of found exponents */
    static size_t count   = 0;       /* number of terms stored */
    static size_t capacity = 0;      /* current allocated capacity */
    static idx_t next_k   = 0;       /* exponent to be processed next */

    /* Decimal big integer for 2^next_k (least‑significant digit first) */
    static unsigned char digits[256];
    static size_t len = 1;           /* number of initialized digits */
    /* initially 2^0 = 1 */
    if (capacity == 0) {
        digits[0] = 1;
        capacity = 16;
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* Grow the terms array if necessary */
    if (n > capacity) {
        while (capacity < n) capacity <<= 1;
        terms = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (count < n) {
        /* Check if current 2^next_k contains digit 2 */
        bool has_two = false;
        for (size_t i = 0; i < len; ++i) {
            if (digits[i] == 2) { has_two = true; break; }
        }
        if (!has_two) {
            terms[count] = (seq_t)next_k;
            ++count;
        }
        /* Prepare next power: multiply current value by 2 */
        unsigned int carry = 0;
        for (size_t i = 0; i < len; ++i) {
            unsigned int t = digits[i] * 2U + carry;
            digits[i] = t % 10U;
            carry = t / 10U;
        }
        while (carry) {
            digits[len++] = carry % 10U;
            carry /= 10U;
        }
        ++next_k;
        /* If the power grows beyond our digit buffer, expand it */
        if (len > sizeof(digits)/sizeof(digits[0])) {
            size_t new_size = sizeof(digits)/sizeof(digits[0]) * 2;
            unsigned char *new_ptr = (unsigned char *)realloc(digits, new_size);
            if (new_ptr == NULL) return (seq_t)0; /* allocation failure */
            /* no need to zero init because we keep using only len elements */
            memcpy(digits, new_ptr, len);
        }
    }

    return terms[n-1];
}
