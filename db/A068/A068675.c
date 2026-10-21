/*
 * A068675  Numbers k such that a prime can be obtained by inserting a 1
 *          before, somewhere inside, or after the digits of k.
 * https://oeis.org/A068675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   digit_len, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A068675: numbers k such that inserting a decimal
   digit '1' somewhere (before, between, or after the digits of k) yields
   a prime.  The routine generates the sequence by checking integers
   sequentially, caching found values for subsequent calls. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_len.h"

static bool candidate_ok(seq_t k)
{
    int d = digit_len(k);          /* number of digits in k            */
    for (int pos = 0; pos <= d; pos++) {
        /* poww = 10^(d-pos) */
        seq_t poww = 1;
        for (int i = 0; i < d - pos; i++) {
            poww *= 10;
        }
        seq_t high = k / poww;     /* digits before insertion          */
        seq_t low  = k % poww;     /* digits after  insertion          */
        /* new number = high * 10^(d-pos+1) + 10^(d-pos) + low */
        seq_t num = high * (poww * 10) + poww + low;
        if (is_prime_big(num)) {
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, behaviour irrelevant */

    /* static cache of found terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* static state for incremental generation */
    static seq_t next_candidate = 1;  /* next integer to test */
    static bool first_call = true;

    if (first_call) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        first_call = false;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (candidate_ok(next_candidate)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
