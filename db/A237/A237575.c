/*
 * A237575  Fibonacci-like numbers with nonincreasing positive digits. Let a**
 *          denote the number that is obtained from a if its digits are
 *          written in nonincreasing order. Let a<+>b = (a + b)**. a(0)=0,
 *          a(1)=1, for n>=2, a(n) = a(n-1) <+> a(n-2).
 * https://oeis.org/A237575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) <+> a(n-2) with a(0)=0, a(1)=1; where a<+>b =
 *            (a+b)**. ** (denoted by two asterisks) sorts the decimal digits
 *            of a+b in non‑increasing order
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 20, u64 n <= 37, u128 n <= 59,
 *            big any n
 * Helpers:   digit_frequencies, digit_len, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/digit_frequencies.h"
#include "../utils/digit_len.h"
#include "../utils/seq_from_str.h"

/* Cache for already computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity of cache  (size_t) */
static idx_t cached_upto = 0;  /* number of terms already filled (next index to compute) */

/* Temporary buffer for building the sorted‑digit string */
static char *tmp_buf = NULL;
static size_t tmp_cap = 0;

/* Return the number obtained by sorting the decimal digits of val in descending order */
static seq_t sort_desc_digits(seq_t val) {
    unsigned int freq[10];
    digit_frequencies(val, freq);

    size_t len = (size_t)digit_len(val);
    if (tmp_cap < len + 1) {
        tmp_cap = len + 1;
        char *newbuf = (char *)realloc(tmp_buf, tmp_cap);
        tmp_buf = newbuf;
    }

    size_t pos = 0;
    for (int d = 9; d >= 0; --d) {
        for (unsigned int c = 0; c < freq[d]; ++c) {
            tmp_buf[pos++] = '0' + d;
        }
    }
    tmp_buf[pos] = '\0';
    return seq_from_str(tmp_buf);
}

/* Ensure cache has space for indices up to n and initialise the first terms if needed */
static void ensure_cache(idx_t n) {
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t newcap = needed * 2;
        seq_t *newc = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i) newc[i] = cache[i];
            free(cache);
        }
        cache = newc;
        cache_cap = newcap;
    }

    if (cached_upto == 0) {
        cache[0] = 0;
        cached_upto = 1;
    }
    if (cached_upto <= 1 && n >= 1) {
        cache[1] = 1;
        cached_upto = 2;
    }
}

seq_t a(idx_t n) {
    ensure_cache(n);

    if ((idx_t)cached_upto > n) {
        return cache[n];
    }

    for (idx_t i = cached_upto; i <= n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        cache[i] = sort_desc_digits(sum);
    }
    cached_upto = n + 1;
    return cache[n];
}
