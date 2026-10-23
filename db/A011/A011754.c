/*
 * A011754  Number of ones in the binary expansion of 3^n.
 * https://oeis.org/A011754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = A000120(3^n) (number of 1‑bits in the binary expansion of
 *            3^n)
 * Range:     u8 n <= 296, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <limits.h>

seq_t a(idx_t n)
{
    /* Special‑case n == 0 */
    if (n == 0) {
        return (seq_t)1; /* 3^0 = 1 has one bit set */
    }

    /* Estimate number of 64‑bit words needed for 3^n:
       bits ≈ n * log2(3) ≈ 1.5849625007 * n
       words = ceil(bits / 64) + a small buffer */
    double bits = n * 1.584962500721156L;
    size_t words = (size_t)(bits / 64.0L) + 2;
    if (words == 0) words = 1;

    uint64_t *digits = (uint64_t *)malloc(words * sizeof(uint64_t));
    if (!digits) {
        /* Allocation failed: return 0 as a safe fallback */
        return (seq_t)0;
    }

    /* Initialise as 1 (3^0) */
    for (size_t i = 0; i < words; ++i) {
        digits[i] = 0;
    }
    digits[0] = 1;
    size_t clen = 1;  /* number of used words */

    /* Multiply by 3, n times */
    for (idx_t i = 0; i < n; ++i) {
        unsigned __int128 carry = 0;
        for (size_t j = 0; j < clen; ++j) {
            unsigned __int128 prod = (unsigned __int128)digits[j] * 3 + carry;
            digits[j] = (uint64_t)prod;
            carry = prod >> 64;
        }
        if (carry) {
            if (clen < words) {
                digits[clen++] = (uint64_t)carry;
            } else {
                /* In the unlikely event the estimate was too small,
                   re‑allocate with one extra slot */
                uint64_t *new_d = (uint64_t *)realloc(digits, (words + 1) * sizeof(uint64_t));
                if (!new_d) {
                    free(digits);
                    return (seq_t)0;
                }
                digits = new_d;
                digits[words++] = (uint64_t)carry;
                ++clen;
            }
        }
    }

    /* Count set bits */
    unsigned long long count = 0;
    for (size_t j = 0; j < clen; ++j) {
        count += __builtin_popcountll(digits[j]);
    }

    free(digits);
    return (seq_t)count;
}
