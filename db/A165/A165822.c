/*
 * A165822  a(n) = the smallest positive integer that, when written in binary,
 *          contains both binary n and the binary representation of the n-th
 *          Fibonacci number as substrings.
 * https://oeis.org/A165822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 41, u64 n <= 90, u128 n <= 177,
 *            big any n
 * Helpers:   bitlen_seq_t, bits_to_seq, consider_candidate, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A165822: smallest positive integer whose binary
   representation contains both binary n and binary Fibonacci(n). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/fib.h"
#include "../utils/bitlen_seq_t.h"
#include "../utils/bits_to_seq.h"
#include "../utils/consider_candidate.h"

#define MAX_BITS 12000   /* enough for the largest n (≈10000) */

/* Return length of overlap between suffix of first and prefix of second. */
static size_t overlap(const uint8_t *first, size_t flen,
                      const uint8_t *second, size_t slen)
{
    size_t kmax = flen < slen ? flen : slen;
    for (size_t k = kmax; k > 0; --k) {
        size_t i;
        for (i = 0; i < k; ++i)
            if (first[flen - k + i] != second[i])
                break;
        if (i == k)
            return k;
    }
    return 0;
}

/* Return true iff bit array a of length la contains b of length lb as a substring. */
static bool contains(const uint8_t *a, size_t la,
                     const uint8_t *b, size_t lb)
{
    if (lb > la)
        return false;
    for (size_t i = 0; i + lb <= la; ++i) {
        if (memcmp(&a[i], b, lb) == 0)
            return true;
    }
    return false;
}

/* Build most‑significant‑first bit array of v. */
static void build_bits(seq_t v, size_t *len, uint8_t *buf)
{
    *len = bitlen_seq_t(v);
    for (size_t i = 0; i < *len; ++i) {
        size_t shift = *len - 1 - i;
        buf[i] = (uint8_t)((v >> shift) & 1U);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* unused; offset starts at 1 */

    seq_t fib_n = fib((idx_t)n);

    uint8_t bits_n[MAX_BITS], bits_fib[MAX_BITS];
    size_t len_n, len_fib;
    build_bits((seq_t)n, &len_n, bits_n);
    build_bits(fib_n, &len_fib, bits_fib);

    uint8_t best_bits[MAX_BITS];
    size_t best_len = SIZE_MAX;
    uint8_t candidate[MAX_BITS];

    /* Case 1: one contains the other. */
    if (contains(bits_n, len_n, bits_fib, len_fib)) {
        consider_candidate(bits_n, len_n, best_bits, &best_len);
    } else if (contains(bits_fib, len_fib, bits_n, len_n)) {
        consider_candidate(bits_fib, len_fib, best_bits, &best_len);
    }

    /* Case 2: n followed by fib with maximum overlap. */
    size_t ov1 = overlap(bits_n, len_n, bits_fib, len_fib);
    size_t len1 = len_n + len_fib - ov1;
    memcpy(candidate, bits_n, len_n);
    memcpy(candidate + len_n, bits_fib + ov1, len_fib - ov1);
    consider_candidate(candidate, len1, best_bits, &best_len);

    /* Case 3: fib followed by n with maximum overlap. */
    size_t ov2 = overlap(bits_fib, len_fib, bits_n, len_n);
    size_t len2 = len_fib + len_n - ov2;
    memcpy(candidate, bits_fib, len_fib);
    memcpy(candidate + len_fib, bits_n + ov2, len_n - ov2);
    consider_candidate(candidate, len2, best_bits, &best_len);

    return bits_to_seq(best_bits, best_len);
}
