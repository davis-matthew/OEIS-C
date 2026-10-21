/*
 * A222813  Numbers whose binary representation is palindromic and in which
 *          all runs of 0's and 1's have length at least 2.
 * https://oeis.org/A222813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 87, u32 n <= 4179, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   runs_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerate binary palindromes with no isolated runs of 0 or 1 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/runs_ok.h"

/* Helper: reverse the lowest 'n' bits of x */
static uint64_t reverse_bits(uint64_t x, int n)
{
    uint64_t rev = 0;
    for (int i = 0; i < n; ++i) {
        rev = (rev << 1) | (x & 1ULL);
        x >>= 1;
    }
    return rev;
}

/* Build the full palindrome from the left half bits */
static inline seq_t palindrome_from_half(uint64_t half, int halfLen, int L)
{
    seq_t val;
    if (L % 2 == 0) {                 /* even length */
        uint64_t rev = reverse_bits(half, halfLen);
        val = ((seq_t)half << halfLen) | (seq_t)rev;
    } else {                          /* odd length */
        uint64_t half_no_mid = half >> 1;      /* drop the middle bit */
        uint64_t rev = reverse_bits(half_no_mid, halfLen - 1);
        val = ((seq_t)half << (halfLen - 1)) | (seq_t)rev;
    }
    return val;
}

/* Dynamic array to cache generated sequence values */
static seq_t *terms         = NULL;
static size_t terms_len     = 0;
static size_t terms_cap     = 0;
static int    last_length   = 1;       /* last length processed */

/* Ensure the cache has enough capacity */
static void ensure_cache_space(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 4;
    while (new_cap < needed) new_cap <<= 1;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* Append a new term to the cache */
static void push_term(seq_t val)
{
    ensure_cache_space(terms_len + 1);
    terms[terms_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1, but guard nonetheless */

    /* Initialize cache on first call */
    if (terms == NULL) {
        terms = (seq_t *)malloc(64 * sizeof(seq_t));
        terms_cap = 64;
        terms_len = 0;
        last_length = 1;
    }

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        int L = last_length + 1;    /* next length to process */

        int halfLen = (L + 1) / 2;
        uint64_t start = 1ULL << (halfLen - 1);          /* ensure leading 1 */
        uint64_t end   = (1ULL << halfLen) - 1;

        for (uint64_t halfBits = start; halfBits <= end; ++halfBits) {
            seq_t val = palindrome_from_half(halfBits, halfLen, L);
            if (runs_ok(val, L))
                push_term(val);
        }
        last_length = L;
    }

    return terms[n - 1];
}
