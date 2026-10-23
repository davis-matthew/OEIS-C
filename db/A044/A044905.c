/*
 * A044905  Numbers whose base-4 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑4 run lengths alternate: odd, even, odd, … */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#if !defined(__cplusplus)
/* ----------------------------------------- */
/* Check whether a number satisfies the rule  */
/* ----------------------------------------- */
static bool good_base4_runs(seq_t x)
{
    /* Extract the base‑4 digits, least significant first. */
    unsigned char *digits = NULL;
    size_t cap   = 0;
    size_t len   = 0;
    seq_t y = x;

    while (y > (seq_t)0) {
        unsigned int d = (unsigned int)(y % (seq_t)4);
        if (len == cap) {
            size_t newcap = cap ? cap * 2 : 8;
            unsigned char *tmp = (unsigned char *)realloc(digits, newcap);
            if (!tmp) { free(digits); return false; }
            digits = tmp; cap = newcap;
        }
        digits[len++] = (unsigned char)d;
        y /= (seq_t)4;
    }
    if (len == 0) { free(digits); return false; }   /* should not happen */

    bool odd_expected = true;          /* first run must be odd */
    unsigned int prev = digits[len-1];
    size_t run = 1;

    if (len >= 2) {
        for (int idx = (int)len - 2; idx >= 0; --idx) {
            unsigned int d = digits[idx];
            if (d == prev) {
                ++run;
            } else {
                if ((run & 1) != odd_expected) { free(digits); return false; }
                odd_expected = !odd_expected;
                prev = d;
                run = 1;
            }
        }
    }

    if ((run & 1) != odd_expected) { free(digits); return false; }
    free(digits);
    return true;
}
#else
/* ----------------------------------------- */
/* C++ implementation (uses std::vector)    */
/* ----------------------------------------- */
#include <vector>

static bool good_base4_runs(seq_t x)
{
    std::vector<unsigned char> digits;
    seq_t y = x;
    while (y > (seq_t)0) {
        digits.push_back((unsigned char)(y % (seq_t)4));
        y /= (seq_t)4;
    }
    if (digits.empty()) return false;

    bool odd_expected = true;
    unsigned int prev = digits.back();
    size_t run = 1;

    for (int idx = (int)digits.size() - 2; idx >= 0; --idx) {
        unsigned int d = digits[idx];
        if (d == prev) {
            ++run;
        } else {
            if ((run & 1) != odd_expected) return false;
            odd_expected = !odd_expected;
            prev = d;
            run = 1;
        }
    }

    if ((run & 1) != odd_expected) return false;
    return true;
}
#endif

/* ----------------------------------------- */
/* Cache and retrieval of the n‑th value     */
/* ----------------------------------------- */

#if !defined(__cplusplus)
static seq_t *cache  = NULL;   /* stored values */
static size_t cap   = 0;       /* allocated capacity */
static size_t sz    = 0;       /* number of stored values */

/* Fill the cache until it contains at least n values. */
static void ensure_array(idx_t n)
{
    static seq_t cand = (seq_t)0;   /* last tested candidate */

    while (sz < (size_t)n) {
        ++cand;                                       /* next integer */
        if (good_base4_runs(cand)) {
            if (sz == cap) {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) { free(cache); exit(EXIT_FAILURE); }
                cache = tmp; cap = newcap;
            }
            cache[sz++] = cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n=0 */
    ensure_array(n);
    return cache[(size_t)(n - (idx_t)1)];
}
#else
static std::vector<seq_t> cache;   /* stored values */

/* Fill the vector until it contains at least n elements. */
static void ensure_vector(idx_t n)
{
    static seq_t cand = (seq_t)0;
    while ((idx_t)cache.size() < n) {
        ++cand;
        if (good_base4_runs(cand))
            cache.push_back(cand);
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n=0 */
    ensure_vector(n);
    return cache[(size_t)(n - (idx_t)1)];
}
#endif
