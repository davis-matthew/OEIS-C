/*
 * A048677  Concatenation of first n palindromic primes.
 * https://oeis.org/A048677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: concatenation of the first n palindromic primes */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* multiply x by 10^e; e is small (≤20 for our use case) */
static seq_t pow10_small(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 10;
    return r;
}

/* number of decimal digits of x (x > 0) */
static int digit_len(seq_t x)
{
    int len = 0;
    while (x > 0) {
        ++len;
        x /= 10;
    }
    return len > 0 ? len : 1;
}

/* generate next palindrome (not necessarily prime).  State is kept static. */
static seq_t next_palindrome(void)
{
    /* static state for the palindrome generator */
    static int  plen  = 1;            /* current palindrome length   */
    static uint64_t start = 1;       /* lower bound of current prefix  */
    static uint64_t end   = 9;       /* upper bound of current prefix  */
    static uint64_t cur   = 1;       /* current prefix value          */

    /* build palindrome from current prefix */
    seq_t pal = (seq_t)cur;
    uint64_t tmp = (plen & 1) ? cur / 10 : cur;   /* skip middle digit for odd length */
    while (tmp > 0) {
        pal = pal * 10 + (tmp % 10);
        tmp /= 10;
    }

    /* advance prefix; if exceeded, increase length */
    ++cur;
    if (cur > end) {
        ++plen;
        int half = (plen + 1) / 2;
        start = 1;
        for (int i = 1; i < half; ++i)
            start *= 10;
        end   = start * 10 - 1;
        cur   = start;
    }
    return pal;
}

/* concatenate prev with p: prev * 10^d + p  */
static seq_t concat_prev_with(seq_t prev, seq_t p)
{
    seq_t pow = pow10_small((idx_t)digit_len(p));
    return prev * pow + p;
}

/* main function computing a(n) */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;     /* cache of computed terms */
    static idx_t   cap    = 0;        /* cache capacity */
    static idx_t   len    = 0;        /* number of terms cached */

    /* Ensure we have at least n terms computed */
    while (len < n) {
        /* find next palindromic prime */
        seq_t p;
        do {
            p = next_palindrome();
        } while (!is_prime_big(p));

        seq_t cur;
        if (len == 0)            /* first term */
            cur = p;
        else
            cur = concat_prev_with(cache[len - 1], p);

        /* grow cache if necessary */
        if (len >= cap) {
            size_t new_cap = (cap == 0) ? 64 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*tmp));
            if (!tmp) {
                /* memory allocation failed; terminate */
                exit(1);
            }
            cache = tmp;
            cap   = new_cap;
        }

        cache[len++] = cur;
    }
    return cache[n - 1];
}
