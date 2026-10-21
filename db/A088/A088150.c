/*
 * A088150  Value of n-th digit (counting from the right) in binary
 *          representation of n^n.
 * https://oeis.org/A088150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(n^n / 2^n) mod 2 (the n‑th rightmost bit of n^n)
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Maximum n that is expected to be handled (≈10 000). */
#define MAX_BITS 10001
#define MAX_WORDS ((MAX_BITS + 63) / 64)   /* 157 */

static void mul_mod(const uint64_t *x, const uint64_t *y, uint64_t *out,
                    size_t words, size_t bits)
{
    /* Temporary array for intermediate 128‑bit products. */
    unsigned __int128 temp[2 * MAX_WORDS] = {0};

    /* Compute full product. */
    for (size_t i = 0; i < words; ++i) {
        for (size_t j = 0; j < words; ++j) {
            temp[i + j] += (unsigned __int128)x[i] * y[j];
        }
    }

    /* Propagate carries and keep only the low 'bits' bits. */
    unsigned __int128 carry = 0;
    for (size_t k = 0; k < words; ++k) {
        unsigned __int128 val = temp[k] + carry;
        out[k] = (uint64_t)val;
        carry = val >> 64;
    }

    /* Mask the upper bits beyond 'bits'. */
    size_t bits_last = bits & 63;
    if (bits_last) {
        out[words - 1] &= ((uint64_t)1 << bits_last) - 1;
    }
}

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 (by the convention 0^0 = 1). */
    if (n == 0)
        return (seq_t)1;

    /* Count of bits needed for the modulus 2^(n+1). */
    size_t bits = n + 1;
    size_t words = (bits + 63) >> 6;   /* 64‑bit limbs */

    /* Allocate working arrays.  The maximum size needed is SMALL. */
    uint64_t *base  = (uint64_t *)malloc(words * sizeof(uint64_t));
    uint64_t *res   = (uint64_t *)malloc(words * sizeof(uint64_t));
    uint64_t *tmp   = (uint64_t *)malloc(words * sizeof(uint64_t));
    if (!base || !res || !tmp)
        return (seq_t)0;                 /* fallback on allocation failure */

    /* Initialise base = n, result = 1. */
    memset(base, 0, words * sizeof(uint64_t));
    base[0] = (uint64_t)n;
    memset(res, 0, words * sizeof(uint64_t));
    res[0] = 1;

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            mul_mod(res, base, tmp, words, bits);
            memcpy(res, tmp, words * sizeof(uint64_t));
        }
        exp >>= 1;
        if (exp) {
            mul_mod(base, base, tmp, words, bits);
            memcpy(base, tmp, words * sizeof(uint64_t));
        }
    }

    /* Extract the n-th bit (0‑based from the right). */
    size_t word_idx = n >> 6;        /* n / 64 */
    size_t bit_idx  = n & 63;        /* n % 64 */
    seq_t bit = (seq_t)((res[word_idx] >> bit_idx) & 1u);

    free(base);
    free(res);
    free(tmp);
    return bit;
}
