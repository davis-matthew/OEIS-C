/*
 * A069881  Numbers n such that n and 2n+1 are both palindromes.
 * https://oeis.org/A069881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 92, u32 n <= 3133, u64 n <= 9235,
 *            u128 n <= 9235, big any n
 * Helpers:   is_palindrome, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069881: numbers n such that n and 2 n + 1 are both palindromes in base 10. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_palindrome.h"
#include "../utils/pow10.h"

/* Helper that constructs a palindrome from the given half and total length. */
static seq_t make_pal(seq_t half, int len)
{
    seq_t pal   = half;
    seq_t temp  = half;
    if (len & 1)       /* odd length – skip the middle digit */
        temp /= 10;
    while (temp > 0)   /* append reverse of temp */
    {
        pal = pal * 10 + (temp % 10);
        temp /= 10;
    }
    return pal;
}

/* Main entry point. */
seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* current state of the palindrome generator */
    static int    cur_len  = 1;   /* current palindrome length */
    static seq_t  cur_half = 0;   /* last used half (0 means start of length) */

    if (cache_cap == 0) {          /* one-time allocation */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* grow cache when needed */
    if (cache_len == cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* generate terms until we reach the requested index */
    while (cache_len < n) {
        /* compute the range of halves for the current length */
        int half_len = (cur_len + 1) >> 1;   /* ceil(cur_len/2) */
        seq_t start_half = pow10((idx_t)(half_len - 1));    /* 10^(half_len-1) */
        seq_t end_half   = start_half * 10 - 1;             /* 10^half_len-1 */

        if (cur_half == 0)          /* first half for this length */
            cur_half = start_half - 1;

        /* advance to the next half */
        cur_half++;
        if (cur_half > end_half) {  /* finished this length -> next length */
            cur_len++;
            cur_half = 0;
            continue;
        }

        /* candidate n is the palindrome constructed from cur_half */
        seq_t cand = make_pal(cur_half, cur_len);

        /* 2*n+1 may overflow for 64‑bit seq_t; guard against that */
#if defined(UINT64_MAX)
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            if (cand > ((UINT64_MAX - 1) / 2))
                continue;          /* 2*cand+1 would overflow 64‑bit; skip */
        }
#endif
        seq_t twice = cand * 2 + 1;
        if (is_palindrome(twice, 10)) {          /* cand is already a palindrome */
            /* cache the term */
            if (cache_len == cache_cap) {         /* grow cache if full */
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
