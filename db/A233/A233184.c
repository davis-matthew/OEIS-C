/*
 * A233184  a(n) is the least number larger than the total number of letters
 *          in the English names for all terms up to and including a(n).
 * https://oeis.org/A233184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = least an integer m such that m > Σ_{i=1}^{n} L(a(i)),
 *            where L(k) is the number of letters in the English name of k
 * Range:     u8 n <= 21, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"   /* seq_realloc */

/* English name lengths for numbers 1–19 */
static const int len_units[20] = {
    0,    /* dummy for 0 */
    3,    /* one   */
    3,    /* two   */
    5,    /* three */
    4,    /* four  */
    4,    /* five  */
    3,    /* six   */
    5,    /* seven */
    5,    /* eight */
    4,    /* nine  */
    3,    /* ten   */
    6,    /* eleven*/
    6,    /* twelve*/
    8,    /* thirteen*/
    8,    /* fourteen*/
    7,    /* fifteen */
    7,    /* sixteen */
    9,    /* seventeen*/
    8,    /* eighteen */
    8     /* nineteen*/
};

/* English name lengths for tens 20–90 */
static const int len_tens[10] = {
    0, /* dummy for 0 */
    0, /* dummy for 1 */
    6, /* twenty  */
    6, /* thirty  */
    5, /* forty   */
    5, /* fifty   */
    5, /* sixty   */
    7, /* seventy */
    6, /* eighty  */
    6  /* ninety  */
};

static const int len_hundred  = 7;  /* "hundred"    */
static const int len_thousand = 8;  /* "thousand"   */
static const int len_million  = 7;  /* "million"    */
static const int len_billion  = 7;  /* "billion"    */

/* Length of the English name of a number < 1000 (no spaces or hyphens). */
static int len_group(uint32_t n)
{
    int len = 0;
    if (n >= 100) {
        uint32_t h = n / 100;
        len += len_units[h];
        len += len_hundred;
        n %= 100;
        if (n == 0)
            return len;
    }
    if (n >= 20) {
        uint32_t t = n / 10;
        len += len_tens[t];
        n %= 10;
    }
    if (n > 0) {
        len += len_units[n];
    }
    return len;
}

/* Length of the English name of any positive integer (no spaces or hyphens). */
static int word_len(uint64_t n)
{
    int total = 0;
    int pos = 0; /* 0: units, 1: thousand, 2: million, 3: billion */
    while (n > 0) {
        uint32_t group = (uint32_t)(n % 1000);
        if (group > 0) {
            int glen = len_group(group);
            if (pos == 0) {
                total += glen;
            } else if (pos == 1) {
                total += glen + len_thousand;
            } else if (pos == 2) {
                total += glen + len_million;
            } else { /* pos >= 3 */
                total += glen + len_billion;
            }
        }
        n /= 1000;
        pos++;
    }
    return total;
}

/* Static cache for memoization. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static idx_t cur_index = 0;    /* number of computed terms */
static seq_t running_sum = 0;  /* Σ L(a(i)) for i < cur_index */

/* Compute the sequence term a(n). */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined but safe for caller protection */
        return (seq_t)0;

    /* Ensure cache holds at least n elements. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n. */
    while (cur_index < n) {
        seq_t S_prev = running_sum;           /* sum of lengths of previous terms */
        uint64_t cand = (uint64_t)(S_prev + 1); /* candidate integer */

        /* Find smallest cand such that cand > S_prev + len(cand). */
        for (;;) {
            int len_c = word_len(cand);
            seq_t threshold = S_prev + (seq_t)len_c;
            if ((seq_t)cand > threshold)
                break;
            cand++;
        }

        seq_t term = (seq_t)cand;
        cache[cur_index++] = term;
        running_sum += (seq_t)word_len(cand);
    }

    return cache[n - 1];
}
