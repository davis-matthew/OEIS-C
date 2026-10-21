/*
 * A079688  Number of 4's in n!.
 * https://oeis.org/A079688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 934, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A079688: Number of 4's in n! */
/* Each call forces a recomputation of n! in base 10^9 and
 * counts the digit '4' in its decimal expansion.
 * This straightforward algorithm runs in O(n·log n) time
 * (≈ 2×10⁷ elementary operations for n = 10000) which
 * meets the time limit. */

#include <stdint.h>
#include <stdlib.h>

#define BASE 1000000000U  /* 10^9, each word holds 9 decimal digits */

/* Return the number of decimal digit '4' in n! */
seq_t a(idx_t n)
{
    /* Special cases */
    if (n < 2)
        return (seq_t)0;

    /* Allocate a dynamic array for factorial words.
     * We use little‑endian order: digits[0] is least significant word. */
    size_t capacity = 256;
    uint32_t *digits = (uint32_t *)malloc(capacity * sizeof(uint32_t));
    if (!digits) return (seq_t)0; /* allocation failure – return 0 */

    digits[0] = 1;            /* 0! = 1 */
    size_t words = 1;         /* number of used words */

    /* Multiply by every integer from 2 to n */
    for (idx_t m = 2; m <= n; ++m) {
        uint64_t carry = 0;
        for (size_t j = 0; j < words; ++j) {
            uint64_t prod = (uint64_t)digits[j] * (uint64_t)m + carry;
            digits[j] = (uint32_t)(prod % BASE);
            carry     = prod / BASE;
        }
        while (carry) {
            if (words == capacity) {
                capacity <<= 1;                           /* grow array */
                uint32_t *tmp = (uint32_t *)realloc(digits, capacity * sizeof(uint32_t));
                if (!tmp) { free(digits); return (seq_t)0; }
                digits = tmp;
            }
            digits[words++] = (uint32_t)(carry % BASE);
            carry /= BASE;
        }
    }

    /* Count digit '4' in the decimal representation */
    seq_t count = 0;
    for (size_t i = 0; i < words; ++i) {
        uint32_t word = digits[i];
        if (i == words - 1) {                     /* most significant word */
            while (word) {
                if ((word % 10) == 4) ++count;
                word /= 10;
            }
        } else {                                  /* other words: always 9 digits */
            int d = 9;
            while (d--) {
                if ((word % 10) == 4) ++count;
                word /= 10;
            }
        }
    }

    free(digits);
    return count;
}
