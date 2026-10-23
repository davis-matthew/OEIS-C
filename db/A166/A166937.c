/*
 * A166937  Positive integers n such that when they are written in binary, the
 *          longest palindromic substring (or at least one of which that is
 *          tied for longest) in binary n begins and ends with a 1.
 * https://oeis.org/A166937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A166937 – integers whose binary representation's longest palindrome starts with 1 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Return true if the longest palindromic substring of binary x starts with 1. */
static bool longest_start_one(seq_t x)
{
    if (x == 0) return true;          /* not used, but safe */
    idx_t L = bitlen_seq_t(x);
    /* extract bits into an array */
    unsigned char *bits = (unsigned char *)malloc((size_t)L);
    if (!bits) return true;           /* fallback, should not happen */
    for (idx_t i = 0; i < L; ++i) {
        bits[i] = (unsigned char)((x >> (L - 1 - i)) & 1);
    }

    idx_t best1 = 0, best0 = 0;

    /* odd length palindromes */
    for (idx_t c = 0; c < L; ++c) {
        idx_t l = c, r = c;
        idx_t len = 1;
        if (bits[l] == 1 && len > best1) best1 = len;
        else if (bits[l] == 0 && len > best0) best0 = len;

        while (l > 0 && r + 1 < L && bits[l - 1] == bits[r + 1]) {
            --l; ++r; len += 2;
            if (bits[l] == 1 && len > best1) best1 = len;
            else if (bits[l] == 0 && len > best0) best0 = len;
        }
    }

    /* even length palindromes */
    for (idx_t c = 1; c < L; ++c) {
        idx_t l = c - 1, r = c;
        if (bits[l] != bits[r]) continue;
        idx_t len = 2;
        if (bits[l] == 1 && len > best1) best1 = len;
        else if (bits[l] == 0 && len > best0) best0 = len;

        while (l > 0 && r + 1 < L && bits[l - 1] == bits[r + 1]) {
            --l; ++r; len += 2;
            if (bits[l] == 1 && len > best1) best1 = len;
            else if (bits[l] == 0 && len > best0) best0 = len;
        }
    }

    free(bits);
    return best1 >= best0;
}

/* Sequence a(n) = n-th integer whose longest binary palindrome starts with 1 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored terms */
    static size_t size = 0;       /* number of stored terms */
    static size_t cap  = 0;       /* allocated capacity */

    if (n == 0) return 0; /* undefined, but avoid underflow */

    /* generate terms until we have n of them */
    if (size < n) {
        seq_t candidate = (size == 0) ? 1 : cache[size - 1] + 1;
        for (size_t i = size; i < n; ++i) {
            while (!longest_start_one(candidate)) {
                candidate = candidate + 1;
            }
            /* store candidate */
            if (size == cap) {
                size_t newcap = (cap == 0) ? 16 : cap * 2;
                seq_t *newarr = (seq_t *)malloc(newcap * sizeof(seq_t));
                if (!newarr) return 0; /* allocation failed */
                memcpy(newarr, cache, size * sizeof(seq_t));
                free(cache);
                cache = newarr;
                cap = newcap;
            }
            cache[size++] = candidate;
            candidate = candidate + 1;
        }
    }
    return cache[n - 1];
}
