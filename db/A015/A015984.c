/*
 * A015984  Six iterations of Reverse and Add are needed to reach a
 *          palindrome.
 * https://oeis.org/A015984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth natural number that requires exactly six
 *            reverse‑and‑add iterations to reach a palindrome (counting the
 *            first iteration as 1). Palindromes are not excluded; a number
 *            that is a palindrome before the sixth iteration is skipped
 * Range:     u8 none, u16 n <= 2, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* standard headers */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* --------------------------------------------------------------------
   Reverse a positive integer in decimal notation.
   -------------------------------------------------------------------- */
static seq_t rev_decimal(seq_t n)
{
    seq_t rev = 0;
    while (n > 0) {
        seq_t d = n % 10;
        rev = rev * 10 + d;
        n /= 10;
    }
    return rev;
}

/* --------------------------------------------------------------------
   Test whether a number is a palindrome in base‑10.
   -------------------------------------------------------------------- */
static bool is_palindrome(seq_t n)
{
    unsigned char dig[64];
    size_t len = 0;
    if (n == 0) {
        dig[0] = 0;
        len  = 1;
    } else {
        seq_t t = n;
        while (t > 0) {
            dig[len++] = (unsigned char)(t % 10);
            t /= 10;
        }
    }
    for (size_t i = 0; i < len / 2; ++i)
        if (dig[i] != dig[len - 1 - i])
            return false;
    return true;
}

/* --------------------------------------------------------------------
   Does the given starting value need exactly six reverse‑and‑add steps
   to become a palindrome?  The first step counts as 1; if a palindrome
   is reached earlier the value is rejected.
   -------------------------------------------------------------------- */
static bool needs_exactly_six(seq_t val)
{
    seq_t x = val;
    for (int step = 1; step <= 6; ++step) {
        seq_t rev = rev_decimal(x);
        x += rev;
        if (step < 6 && is_palindrome(x))
            return false;
    }
    return is_palindrome(x);
}

/* --------------------------------------------------------------------
   Sequence function: returns the nth element of A015984.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector */
    static std::vector<seq_t> cache;
    static idx_t next_candidate = 0;
    while (cache.size() <= n) {
        seq_t val = static_cast<seq_t>(next_candidate);
        if (needs_exactly_six(val))
            cache.push_back(val);
        ++next_candidate;
    }
    return cache[(size_t)n];
#else
    /* C implementation with manual allocation */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 0;
    while (cache_len <= n) {
        seq_t val = (seq_t)next_candidate;
        if (needs_exactly_six(val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        ++next_candidate;
    }
    return cache[(size_t)n];
#endif
}
