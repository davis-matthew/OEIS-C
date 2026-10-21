/*
 * A088154  Value of n-th digit in duodecimal representation of n^n.
 * https://oeis.org/A088154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor( n^n / 12^n ) mod 12
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Return the n-th digit (base‑12) of n^n, with a(0)=1. */
seq_t a(idx_t n) {
    /* Special case */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Dynamically grow a base‑12 big integer represented by
       an array of digits (least significant first). */
    static uint8_t *digits = NULL;
    static size_t  capacity = 0;
    size_t  dlen = 1;                 /* current number of digits */

    /* Initialise with 1. */
    if (!digits) {
        capacity = 16;
        digits = (uint8_t*)malloc(capacity);
        if (!digits) return (seq_t)0;  /* Out of memory – unlikely in tests */
    }
    digits[0] = 1;

    /* Multiply by n, n times, building n^n. */
    for (idx_t k = 1; k <= n; ++k) {
        uint32_t carry = 0;
        for (size_t i = 0; i < dlen; ++i) {
            uint32_t prod = (uint32_t)digits[i] * (uint32_t)n + carry;
            digits[i] = (uint8_t)(prod % 12);
            carry     = prod / 12;
        }
        while (carry) {
            /* Grow the array if necessary */
            if (dlen == capacity) {
                capacity = capacity * 2 + 16;
                uint8_t *tmp = (uint8_t*)realloc(digits, capacity);
                if (!tmp) return (seq_t)0; /* Out of memory */
                digits = tmp;
            }
            digits[dlen++] = (uint8_t)(carry % 12);
            carry          /= 12;
        }
    }

    /* The desired digit is at position n (0‑based). */
    if (n < dlen) {
        return (seq_t)digits[n];
    } else {
        return (seq_t)0; /* Should not occur for valid n */
    }
}
