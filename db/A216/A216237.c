/*
 * A216237  Happy palindromic numbers.
 * https://oeis.org/A216237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 99, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A216237: happy palindromic numbers.
   The nth term is the nth positive integer that is both
   palindromic (in base 10) and happy (in base 10). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Helper: test whether a 64‑bit integer is happy                     */
/* ------------------------------------------------------------------ */
static bool is_happy_uint64(uint64_t x)
{
    /* 0 is not happy; loop until reaching 1 or the
       known unhappy cycle value 4. */
    if (x == 0)
        return false;
    while (x != 1 && x != 4) {
        uint64_t sum = 0;
        while (x > 0) {
            uint64_t d = x % 10;
            sum += d * d;
            x /= 10;
        }
        x = sum;
    }
    return x == 1;
}

/* ------------------------------------------------------------------ */
/* Helper: compute 10^e as a 64‑bit integer (e <= 19).               */
/* ------------------------------------------------------------------ */
static uint64_t pow10_uint64(int e)
{
    uint64_t r = 1;
    for (int i = 0; i < e; ++i)
        r *= 10ULL;
    return r;
}

/* ------------------------------------------------------------------ */
/* Helper: build a palindrome from the “left” half.                    */
/* If odd==true, the left half contains the middle digit.              */
/* ------------------------------------------------------------------ */
static uint64_t make_palindrome(uint64_t left, bool odd)
{
    uint64_t pal = left;
    uint64_t t   = odd ? left / 10ULL : left;
    while (t > 0) {
        pal = pal * 10ULL + (t % 10ULL);
        t  /= 10ULL;
    }
    return pal;
}

/* ------------------------------------------------------------------ */
/* Main sequence routine.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;               /* undefined for n < 1 */

    /* Static cache of discovered terms. */
    static seq_t *cache        = NULL;
    static size_t cache_len    = 0;
    static size_t cache_cap    = 0;

    /* State used while generating palindromes. */
    static size_t gen_len      = 1;     /* current palindrome length */
    static uint64_t left_start = 1;     /* first left half for gen_len */
    static uint64_t left_end   = 9;     /* last  left half for gen_len */
    static uint64_t left_val   = 1;     /* current left half     */

    /* Allocate/expand cache on first entry. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Keep generating while we don't yet have n terms. */
    while (cache_len < (size_t)n) {
        uint64_t pal = make_palindrome(left_val, (gen_len & 1U) != 0);
        if (is_happy_uint64(pal)) {
            /* Store palindrome as seq_t. */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)pal;
        }

        /* Advance to next left half. */
        ++left_val;
        if (left_val > left_end) {
            /* Move to the next length. */
            ++gen_len;
            int half = (gen_len + 1) / 2;
            left_start = pow10_uint64(half - 1);
            left_end   = pow10_uint64(half) - 1;
            left_val   = left_start;
        }
    }

    return cache[n - 1];
}
