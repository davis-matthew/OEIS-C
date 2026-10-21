/*
 * A061785  a(n) = m such that 2^m < 5^n < 2^(m+1).
 * https://oeis.org/A061785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*log_2(5))
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Beatty sequence ⌊n·log₂(5)⌋ by multiplying 5^n and taking
 * the bit‑length of the result (bit‑length minus one).  No floating point
 * arithmetic is used. */

#ifndef SEQ_A061785_H
#define SEQ_A061785_H

#include <stdint.h>
#include <stdlib.h>

/* The function is expected to be linked with the types seq_t and idx_t
 * defined by the build system.  No static keyword is used so the function
 * has external linkage. */
seq_t a(idx_t n)
{
    /* Handle the offset (n starts at 1).  For n == 0 we return 0
     * which is the value that would be produced for a(0) if the
     * definition were extended.  This does not affect the requested
     * sequence for n >= 1. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Estimate the number of 32‑bit words needed to store 5^n.
     * Since log₂(5) < 2.5, 5^n requires at most 2.5*n bits.
     * Adding a small safety margin allows a fixed allocation that
     * works for the required n ≤ 10000. */
    size_t bits_est = (size_t)(n * 5 / 2 + 5);          /* ≤ 2.5*n + 5 */
    size_t word_cnt = bits_est / 32 + 2;                /* words of 32 bits */

    /* Allocate the big integer buffer. */
    uint32_t *a = (uint32_t *)malloc(word_cnt * sizeof(uint32_t));
    if (!a) {
        /* Out of memory – return 0 as a safe fallback. */
        return (seq_t)0;
    }

    /* Initialise big integer to 1. */
    size_t len = 1;
    a[0] = 1;

    /* Multiply by 5, n times, keeping the result in little‑endian form. */
    for (idx_t i = 0; i < n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t prod = (uint64_t)a[j] * 5 + carry;
            a[j] = (uint32_t)prod;
            carry  = prod >> 32;
        }
        if (carry) {
            a[len++] = (uint32_t)carry;
        }
    }

    /* Compute the bit‑length of the final big integer. */
    uint32_t msword = a[len - 1];
    /* __builtin_clz works only for unsigned int; cast to unsigned. */
    unsigned int leading = __builtin_clz(msword);
    unsigned int word_bits = 32 - leading;            /* bits in msword */

    /* The desired value is bit‑length minus one. */
    seq_t result = (seq_t)( (len - 1) * 32 + (word_bits - 1) );

    free(a);
    return result;
}

#endif /* SEQ_A061785_H */
