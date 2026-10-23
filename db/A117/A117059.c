/*
 * A117059  Palindromes for which the multiplicative digital root is a prime.
 * https://oeis.org/A117059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   multiplicative_digital_root, prime_single_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A117059: palindromes whose multiplicative digital root is prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/multiplicative_digital_root.h"
#include "../utils/prime_single_digit.h"

/* Helper to build a palindrome from a prefix.
   If odd==true, the middle digit of the palindrome is the last
   digit of the prefix. */
static seq_t make_palindrome(seq_t prefix, bool odd)
{
    seq_t pal = prefix;
    seq_t tmp = prefix;
    if (odd) tmp /= 10;
    while (tmp > 0) {
        pal = pal * 10 + (tmp % 10);
        tmp /= 10;
    }
    return pal;
}

/* Main function.  It keeps an internal cache of found terms, and
   sequentially generates palindromes until it has enough terms. */
seq_t a(idx_t n)
{
    /* Cache of found sequence values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* State of the palindrome generator. */
    static idx_t cur_len = 0;          /* current palindrome length */
    static seq_t cur_prefix_start = 0; /* first prefix of current length */
    static seq_t cur_prefix_end   = 0; /* last prefix of current length  */
    static seq_t cur_prefix       = 0; /* current prefix being used    */
    static bool  cur_odd          = false;  /* length odd/even */

    /* Lazy initialization of the cache. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort(); /* Out of memory */
    }

    /* Produce terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Initialise generator on first use or when a new length
           must be started. */
        if (cur_len == 0) {
            cur_len = 1;
            cur_odd = true;
            cur_prefix_start = 1;
            cur_prefix_end   = 9;
            cur_prefix       = cur_prefix_start;
        }

        /* Build the next palindrome of the current length. */
        seq_t pal = make_palindrome(cur_prefix, cur_odd);

        /* Advance to next prefix for future iterations. */
        if (cur_prefix < cur_prefix_end) {
            ++cur_prefix;
        } else {
            /* All prefixes of this length exhausted → advance length. */
            ++cur_len;
            cur_odd = (cur_len % 2) != 0;
            /* Determine new start/end prefixes. */
            seq_t half = (cur_len + 1) / 2;     /* number of digits in prefix   */
            seq_t start = 1;
            for (idx_t i = 1; i < half; ++i) start *= 10;
            seq_t end = start * 10 - 1;
            cur_prefix_start = start;
            cur_prefix_end   = end;
            cur_prefix       = start;
        }

        /* Test whether the palindrome qualifies. */
        seq_t mdr = multiplicative_digital_root(pal);
        if (prime_single_digit(mdr)) {
            /* Store it in the cache. */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort(); /* Out of memory */
                cache = tmp;
            }
            cache[cache_len++] = pal;
        }
    }

    return cache[(size_t)n - 1];
}
