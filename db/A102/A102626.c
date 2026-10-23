/*
 * A102626  2^n written in base n+1.
 * https://oeis.org/A102626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = representation of 2^n in base (n+1) interpreted as a
 *            decimal integer. a(0) = 1 by convention
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Power of two using fast exponentiation.  Works for seq_t being an unsigned
   integer type or an arbitrary‑precision class that supports the usual
   arithmetic operators. */
static seq_t pow2(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 2;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Convention for n == 0. */
    if (n == 0) return (seq_t)1;

    /* Base for the representation. */
    seq_t base = (seq_t)(n + 1);

    /* Compute 2^n. */
    seq_t val = pow2(n);

    /* Collect digits of val in base (n+1).  Dynamic array of uint16_t. */
    size_t cap = 16;
    size_t len = 0;
    uint16_t *digits = (uint16_t *)malloc(cap * sizeof(uint16_t));
    if (!digits) return 0; /* Allocation failure: return 0 as a sentinel. */

    while (val != 0) {
        /* remainder fits into uint16_t because val % base < base <= n+1 <= 10001
           for the range of n we care about. */
        seq_t rem = val % base;
        if (len == cap) {
            cap <<= 1;
            uint16_t *tmp = (uint16_t *)realloc(digits, cap * sizeof(uint16_t));
            if (!tmp) { free(digits); return 0; }
            digits = tmp;
        }
        digits[len++] = (uint16_t)(unsigned long long)rem;
        val /= base;
    }

    /* Build decimal value from the collected digits (most significant first). */
    seq_t result = 0;
    for (size_t i = len; i > 0; --i) {
        result = result * 10 + digits[i - 1];
    }

    free(digits);
    return result;
}
