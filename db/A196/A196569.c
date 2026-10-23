/*
 * A196569  Binomial coefficients of the form C(n, 4) that is a concatenation
 *          of binomial coefficients of the form C(n, 4).
 * https://oeis.org/A196569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, bref
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   binom, binom_contains
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A196569: a(n) is the n-th binomial coefficient C(m,4)
   that can be represented as a concatenation of two or more
   binomial coefficients of the form C(k,4) in decimal notation. */

/* --------------------------------------------------------------------
   Necessary includes – only standard C headers are used.
   ----------------------------------------------------------- */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Include the binomial helper from the utils package. */
#include "../utils/binom.h"
#include "../utils/binom_contains.h"

/* --------------------------------------------------------------------
   Convert a seq_t value to a decimal string.
   ----------------------------------------------------------- */
static void seq_to_dec(seq_t v, char *buf, size_t *len)
{
    size_t i = 0;
    seq_t temp = v;
    if (temp == 0) {
        buf[i++] = '0';
    } else {
        while (temp > 0) {
            unsigned int d = (unsigned int)(temp % (seq_t)10);
            buf[i++] = '0' + d;
            temp /= (seq_t)10;
        }
        /* reverse the string in place */
        for (size_t l = 0, r = i - 1; l < r; ++l, --r) {
            char c = buf[l];
            buf[l] = buf[r];
            buf[r] = c;
        }
    }
    *len = i;
}

/* --------------------------------------------------------------------
   Test whether a value is a concatenation of two or more binomial values.
   ----------------------------------------------------------- */
static bool is_concatable(seq_t val, const seq_t *binom_arr, size_t binom_len)
{
    /* Max digits for C(m,4) with m up to about 100000 is < 20.
       Use a fixed-size buffer large enough for safety. */
    char buf[64];
    size_t len;
    seq_to_dec(val, buf, &len);

    /* DP arrays – length limited by the buffer size. */
    int parts[65];
    for (size_t i = 0; i <= len; ++i)
        parts[i] = -1;
    parts[0] = 0;                       /* zero parts up to position 0 */

    for (size_t i = 0; i < len; ++i) {
        if (parts[i] < 0) continue;     /* unreachable prefix */

        /* Handle leading '0' specially – only single-digit zero is allowed. */
        if (buf[i] == '0') {
            size_t j = i + 1;           /* substring "0" */
            if (j <= len) {
                seq_t sub = 0;
                if (binom_contains(binom_arr, binom_len, sub)) {
                    if (parts[j] < parts[i] + 1)
                        parts[j] = parts[i] + 1;
                }
            }
            continue;                   /* longer substrings would have a leading zero */
        }

        /* Normal case: accumulate the numeric value of the substring. */
        seq_t sub = 0;
        for (size_t j = i + 1; j <= len; ++j) {
            sub = sub * (seq_t)10 + (seq_t)(buf[j - 1] - '0');
            if (binom_contains(binom_arr, binom_len, sub)) {
                if (parts[j] < parts[i] + 1)
                    parts[j] = parts[i] + 1;
            }
        }
    }
    return parts[len] >= 2;
}

/* --------------------------------------------------------------------
   Main sequence function.
   ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static state for memoisation. */
    static int initialized = 0;
    static idx_t next_m = 4;        /* next value of m to test */

#if defined(__cplusplus) && __cplusplus >= 201103L
    /* C++ build – use std::vector for dynamic storage. */
    static std::vector<seq_t> binom_vec;
    static std::vector<seq_t> seq_vec;
    if (!initialized) {
        binom_vec.reserve(64);
        seq_vec.reserve(64);
        /* binomials start with C(0,4) = 0, which is needed for '0' parts. */
        binom_vec.push_back((seq_t)0);
        initialized = 1;
    }

    /* Generate terms until we have at least n elements. */
    while (seq_vec.size() < (size_t)n) {
        seq_t val = binom(next_m, 4);          /* C(next_m,4) */
        if (is_concatable(val,
                          binom_vec.data(),
                          binom_vec.size()))
            seq_vec.push_back(val);
        binom_vec.push_back(val);
        ++next_m;
    }
    return seq_vec[n - 1];
#else
    /* C build – use manual dynamic arrays. */
    static seq_t *binom_vals = NULL;
    static size_t binom_len = 0, binom_cap = 0;
    static seq_t *seq_vals   = NULL;
    static size_t seq_len   = 0, seq_cap   = 0;

    if (!initialized) {
        binom_cap = 64;
        seq_cap   = 64;
        binom_vals = (seq_t *)malloc(binom_cap * sizeof(seq_t));
        seq_vals   = (seq_t *)malloc(seq_cap   * sizeof(seq_t));

        /* binomials start with C(0,4) = 0 */
        binom_vals[0] = (seq_t)0;
        binom_len = 1;
        initialized = 1;
    }

    /* Grow arrays as needed and generate terms. */
    while (seq_len < (size_t)n) {
        /* Ensure there is space for the next binomial value. */
        if (binom_len == binom_cap) {
            binom_cap <<= 1;
            binom_vals = (seq_t *)realloc(binom_vals, binom_cap * sizeof(seq_t));
        }

        /* Compute next binomial value. */
        seq_t val = binom(next_m, 4);

        /* Test for concatenation and store if appropriate. */
        if (is_concatable(val, binom_vals, binom_len)) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
            }
            seq_vals[seq_len++] = val;
        }

        /* Append the newly computed value to the binom array. */
        binom_vals[binom_len++] = val;
        ++next_m;
    }
    return seq_vals[n - 1];
#endif
}
