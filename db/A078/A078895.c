/*
 * A078895  Palindromes which are a nontrivial product of 3 palindromes (which
 *          need not be distinct).
 * https://oeis.org/A078895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078895: palindromes that are a nontrivial product of three
   palindromes (which need not be distinct).  The function generates
   palindromes in ascending order and tests each for a factorisation
   a*b*c with a,b,c > 1.  The first 24 terms given in the problem
   statement are produced in the correct order. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"

static seq_t *pal_list   = NULL;
static size_t pal_len   = 0;
static size_t pal_cap   = 0;

static seq_t *seq_vals   = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;

static idx_t  cur_len    = 0;     /* length of current palindromes */
static seq_t  cur_half_start = 0; /* first left half of current length */
static seq_t  cur_half_end   = 0; /* last left half of current length */
static seq_t  cur_half      = 0;  /* current left half being processed */
static bool   cur_odd       = false; /* whether current length is odd */

/* integer power of 10 (up to a few dozen digits, safe for seq_t) */
static inline seq_t pow10_int(idx_t e)
{
    seq_t v = 1;
    while (e--) v *= 10;
    return v;
}

/* construct a palindrome from a left half and a flag indicating
   whether the overall length is odd (middle digit not duplicated). */
static seq_t build_pal(seq_t half, bool odd)
{
    seq_t pal = half;
    seq_t rev = odd ? half / 10 : half;  /* drop middle digit if odd */
    while (rev > 0) {
        pal = pal * 10 + (rev % 10);
        rev /= 10;
    }
    return pal;
}

/* test whether m can be factored as a product of three palindromes > 1.
   Only palindromes generated so far (in pal_list) are examined. */
static bool has_three_pal_factors(seq_t m)
{
    /* a loop over possible first factor */
    for (size_t i = 0; i < pal_len; ++i) {
        seq_t a = pal_list[i];
        if (a <= 1) continue;
        if (m % a != 0) continue;
        seq_t rem = m / a;

        /* b loop over possible second factor (allow repetitions) */
        for (size_t j = i; j < pal_len; ++j) {
            seq_t b = pal_list[j];
            if (b <= 1) continue;
            if (b > rem) break;          /* b too large */
            if (rem % b != 0) continue;
            seq_t c = rem / b;
            if (c <= 1) continue;
            if (is_palindrome(c, 10)) return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    /* initialise static structures on first call */
    static bool initialized = false;
    if (!initialized) {
        pal_cap = 16;
        pal_list = (seq_t *)malloc(pal_cap * sizeof(seq_t));
        seq_cap = 16;
        seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        /* start with 1‑digit palindromes */
        cur_len = 1;
        cur_odd = true;
        cur_half_start = 1;
        cur_half_end   = 9;
        cur_half       = cur_half_start;
        initialized = true;
    }

    /* generate palindromes until we have enough terms */
    while (seq_len < (size_t)n) {
        seq_t pal = build_pal(cur_half, cur_odd);

        /* store for future factor checks */
        if (pal_len == pal_cap) {
            pal_cap *= 2;
            pal_list = (seq_t *)realloc(pal_list, pal_cap * sizeof(seq_t));
        }
        pal_list[pal_len++] = pal;

        /* check if this palindrome is the desired type */
        if (pal > 1 && has_three_pal_factors(pal)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
            }
            seq_vals[seq_len++] = pal;
        }

        /* advance to next palindrome */
        ++cur_half;
        if (cur_half > cur_half_end) {      /* finished current length */
            ++cur_len;
            size_t half_len = (cur_len + 1) / 2;
            cur_half_start = (half_len == 1) ? 1 : pow10_int(half_len - 1);
            cur_half_end   = pow10_int(half_len) - (seq_t)1;
            cur_half       = cur_half_start;
            cur_odd        = (cur_len % 2 == 1);
        }
    }

    return seq_vals[n - 1];
}
