/*
 * A106432  Levenshtein distance between successive powers of 2 in decimal
 *          representation.
 * https://oeis.org/A106432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 97 terms through n = 968 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Levenshtein distance between the decimal strings of 2^n
   and 2^(n+1). This implements the OEIS definition of a(n). */
#include <stddef.h>
#include <stdint.h>

/* Build the decimal digits of 2^exp into dig (least‑significant first). */
static inline void build_pow2_digits(idx_t exp,
                                     unsigned char *dig,
                                     size_t *len)
{
    dig[0] = 1;
    *len   = 1;

    for (size_t i = 0; i < (size_t)exp; ++i) {
        unsigned int carry = 0;
        for (size_t j = 0; j < *len; ++j) {
            unsigned int v = dig[j] * 2u + carry;
            dig[j]   = (unsigned char)(v % 10u);
            carry    = v / 10u;
        }
        while (carry) {
            dig[*len] = (unsigned char)(carry % 10u);
            carry     = carry / 10u;
            ++(*len);
        }
    }
}

/* Double the number stored in src of length src_len into dst, storing
   the result in dst and updating dst_len. */
static inline void copy_and_double(size_t src_len,
                                   const unsigned char *src,
                                   unsigned char *dst,
                                   size_t *dst_len)
{
    *dst_len = src_len;
    for (size_t i = 0; i < src_len; ++i)
        dst[i] = src[i];

    unsigned int carry = 0;
    for (size_t i = 0; i < *dst_len; ++i) {
        unsigned int v = dst[i] * 2u + carry;
        dst[i]   = (unsigned char)(v % 10u);
        carry    = v / 10u;
    }
    while (carry) {
        dst[*dst_len] = (unsigned char)(carry % 10u);
        carry /= 10u;
        ++(*dst_len);
    }
}

/* Maximum number of decimal digits needed for 2^10000 (~3011 digits). */
#define MAXD 4000

/* Compute a(n).  Uses a single‑row dynamic programming for Levenshtein
   distance on the digit arrays, which are stored least‑significant
   first – the reversal of both strings does not affect the distance. */
seq_t a(idx_t n)
{
    unsigned char d1[MAXD], d2[MAXD];
    size_t len1, len2;
    unsigned int prev[MAXD + 1] = {0};
    unsigned int cur[MAXD + 1]  = {0};

    /* 2^n */
    build_pow2_digits(n, d1, &len1);

    /* 2^(n+1) from 2^n by one extra doubling */
    copy_and_double(len1, d1, d2, &len2);

    /* Initialize first row of DP */
    for (size_t j = 0; j <= len2; ++j)
        prev[j] = (unsigned int)j;

    /* Process each digit of 2^n (least‑significant first) */
    for (size_t i = 1; i <= len1; ++i) {
        cur[0] = (unsigned int)i;
        for (size_t j = 1; j <= len2; ++j) {
            unsigned int ins = prev[j] + 1;
            unsigned int del = cur[j - 1] + 1;
            unsigned int sub = prev[j - 1] + (d1[i - 1] != d2[j - 1] ? 1 : 0);
            unsigned int mn = ins < del ? ins : del;
            cur[j] = mn < sub ? mn : sub;
        }
        for (size_t j = 0; j <= len2; ++j)
            prev[j] = cur[j];
    }

    return (seq_t)prev[len2];
}
