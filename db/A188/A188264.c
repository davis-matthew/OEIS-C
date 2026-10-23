/*
 * A188264  Numbers m that are divisible by the product of the factorials of
 *          their digits in base 10.
 * https://oeis.org/A188264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   sequence A188264 – numbers m such that m is divisible by the
 *            product of the factorials of its decimal digits. This
 *            implementation builds the sequence on demand, caching found
 *            terms until the requested index is available. The property is
 *            tested by comparing prime exponents of 2, 3, 5 and 7 in m wi...
 * Range:     u8 n <= 20, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* exponents of 2,3,5,7 in d! for d=0..9 */
static const uint8_t fact_exp[10][4] = {
    {0,0,0,0},   /* 0! */
    {0,0,0,0},   /* 1! */
    {1,0,0,0},   /* 2! */
    {1,1,0,0},   /* 3! */
    {3,1,0,0},   /* 4! */
    {3,1,1,0},   /* 5! */
    {4,2,1,0},   /* 6! */
    {4,2,1,1},   /* 7! */
    {7,2,1,1},   /* 8! */
    {7,4,1,1}    /* 9! */
};

/* Test whether a given uint64_t value satisfies the divisibility
   condition.  The test uses only the primes 2,3,5,7 (which are the
   only primes appearing in factorials up to 9!). */
static int satisfies(uint64_t m) {
    uint8_t need2 = 0, need3 = 0, need5 = 0, need7 = 0;
    uint64_t t = m;
    /* accumulate required exponents from the digits */
    while (t > 0) {
        unsigned d = t % 10;
        need2 += fact_exp[d][0];
        need3 += fact_exp[d][1];
        need5 += fact_exp[d][2];
        need7 += fact_exp[d][3];
        t /= 10;
    }

    /* compute actual exponents in m */
    uint8_t have2 = 0, have3 = 0, have5 = 0, have7 = 0;
    t = m;
    while (t % 2 == 0) { have2++; t /= 2; }
    t = m;
    while (t % 3 == 0) { have3++; t /= 3; }
    t = m;
    while (t % 5 == 0) { have5++; t /= 5; }
    t = m;
    while (t % 7 == 0) { have7++; t /= 7; }

    return have2 >= need2 && have3 >= need3 &&
           have5 >= need5 && have7 >= need7;
}

/* Main sequence function.  It lazily builds a cache of terms until the
   requested index n is available.  The function is thread‑unsafe
   (as per typical OEIS library conventions) but satisfies the
   required external linkage. */
seq_t a(idx_t n) {
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  len    = 0;
    static uint64_t last_checked = 0;
    /* allocate initial storage */
    if (cache == NULL) {
        cap   = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }
    /* keep generating terms until we have at least n of them */
    while (len < n) {
        ++last_checked;
        if (satisfies(last_checked)) {
            if (len == cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
