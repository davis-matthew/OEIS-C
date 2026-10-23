/*
 * A144098  a(1)=1. a(n) = the smallest positive multiple of a(n-1) that
 *          contains the binary representations of all positive integers <= n
 *          at least once each in its binary representation.
 * https://oeis.org/A144098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_to_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=smallest multiple of a(n-1) containing binary representations of 1..n */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_to_bits.h"

/* Forward declarations */
static void ensure_binary_cap(size_t n);
static const char *binary_str_of(size_t x);
static void ensure_a_cap(size_t n);
static seq_t find_smallest_multiple(seq_t base, size_t n);

/* memoization storage for the sequence itself */
static seq_t *a_vals   = NULL;
static size_t a_cap    = 0;    /* allocated capacity, number of slots */
static size_t a_len    = 0;    /* highest index computed so far */

/* memoization storage for binary strings of integers */
static char **bin_strs = NULL;
static size_t bin_cap  = 0;    /* allocated capacity for bin_strs */

/* Ensure binary string table has capacity >= n+1 */
static void ensure_binary_cap(size_t n)
{
    if (bin_cap <= n) {
        size_t new_cap = bin_cap ? bin_cap * 2 : 16;
        while (new_cap <= n) new_cap <<= 1;
        char **tmp = (char**)realloc(bin_strs, new_cap * sizeof(char *));
        if (!tmp) exit(EXIT_FAILURE);
        bin_strs = tmp;
        for (size_t i = bin_cap; i < new_cap; ++i) bin_strs[i] = NULL;
        bin_cap = new_cap;
    }
}

/* Return binary representation of x as a newly allocated string
   terminated with '\0'. The returned pointer is cached. */
static const char *binary_str_of(size_t x)
{
    if (x >= bin_cap) ensure_binary_cap(x);
    if (bin_strs[x]) return bin_strs[x];

    /* Build reversed bit string */
    size_t len = 0;
    char tmp[64];
    size_t t = x;
    if (t == 0) {
        tmp[len++] = '0';
    } else {
        while (t) {
            tmp[len++] = (t & 1) ? '1' : '0';
            t >>= 1;
        }
    }

    char *s = (char*)malloc(len + 1);
    if (!s) exit(EXIT_FAILURE);
    for (size_t i = 0; i < len; ++i) s[i] = tmp[len - 1 - i];
    s[len] = '\0';
    bin_strs[x] = s;
    return s;
}

/* Ensure the a_vals array can hold indices up to n */
static void ensure_a_cap(size_t n)
{
    if (a_cap <= n) {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_vals = tmp;
        a_cap = new_cap;
    }
}

/* Compute the smallest multiple of 'base' that contains all
   binary patterns 1..n. */
static seq_t find_smallest_multiple(seq_t base, size_t n)
{
    seq_t cand = base;        /* candidate = base * 1 */
    /* Precompute binary patterns 1..n */
    for (size_t i = 1; i <= n; ++i) binary_str_of(i);

    while (1) {
        /* Convert cand to binary string */
        char *bits;
        size_t bits_len;
        seq_to_bits(cand, &bits, &bits_len);

        /* Check all patterns */
        bool ok = true;
        for (size_t i = 1; i <= n; ++i) {
            if (!strstr(bits, bin_strs[i])) {
                ok = false;
                break;
            }
        }
        free(bits);
        if (ok) return cand;

        /* Next candidate: add base */
        cand += base;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for 0, safety */

    /* Ensure memoization tables large enough */
    if (a_len == 0) {
        ensure_a_cap(1);
        a_vals[1] = (seq_t)1;
        a_len = 1;
    }
    if (n > a_len) {
        ensure_a_cap(n);
        for (size_t i = a_len + 1; i <= n; ++i) {
            if (i == 1) {
                a_vals[1] = (seq_t)1;
                continue;
            }
            /* Compute a(i) */
            seq_t prev = a_vals[i - 1];
            a_vals[i] = find_smallest_multiple(prev, i);
        }
        a_len = n;
    }
    return a_vals[(size_t)n];
}
