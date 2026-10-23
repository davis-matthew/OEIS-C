/*
 * A022506  Describe previous term from the right (method A - initial term is
 *          0).
 * https://oeis.org/A022506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(0)=0; for n≥1, a(n)=describe_right(a(n-1)) where
 *            describe_right(x) is obtained by reading the decimal
 *            representation of x from the right, grouping equal consecutive
 *            digits, and concatenating for each group the count followed by
 *            the digit
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   digit_len, get_digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Included utilities for digit handling and power of ten */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_len.h"
#include "../utils/get_digits.h"
#include "../utils/pow10.h"

/* Build next term by describing the current term from the right. */
static seq_t describe_right(seq_t x)
{
    /* Buffer for extracted digits (least‑significant first). */
    static uint8_t *digit_buf = NULL;
    static size_t  digit_buf_cap = 0;

    /* Estimate required buffer size: number of digits of x plus one. */
    size_t est_len = digit_len(x) + 1;
    if (digit_buf_cap < est_len) {
        /* Cast required for C++ compatibility. */
        uint8_t *tmp = (uint8_t *)realloc(digit_buf, est_len * sizeof(uint8_t));
        if (!tmp) return (seq_t)0;                /* out‑of‑memory fallback */
        digit_buf = tmp;
        digit_buf_cap = est_len;
    }

    /* Retrieve digits (leap‑significant first). */
    size_t len = 0;
    get_digits(x, digit_buf, &len);

    seq_t result = (seq_t)0;
    size_t i = 0;
    while (i < len) {
        uint8_t curr = digit_buf[i];
        size_t  count = 1;
        ++i;
        while (i < len && digit_buf[i] == curr) {
            ++count;
            ++i;
        }

        seq_t freq  = (seq_t)count;                     /* frequency */
        seq_t group = freq * ((seq_t)10) + (seq_t)curr; /* concatenate */

        int group_len   = digit_len(freq) + 1;           /* digits in group */
        seq_t pow10d    = pow10((idx_t)group_len);      /* 10^group_len   */

        result = result * pow10d + group;               /* append group   */
    }
    return result;
}

/* Sequence function with memoisation. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache for a(0)=0. */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* out‑of‑memory fallback */
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Return cached value if already computed. */
    if (n < cache_len) return cache[n];

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t next  = describe_right(prev);

        /* Resize cache. */
        seq_t *tmp = (seq_t *)realloc(cache, (i + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* out‑of‑memory fallback */
        cache = tmp;

        cache[i] = next;
    }
    cache_len = n + 1;
    return cache[n];
}
