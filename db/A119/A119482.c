/*
 * A119482  Numbers that are diminished by taking its sum of letters (writing
 *          out its English name and adding the letters using a=1, b=2, c=3,
 *          ...).
 * https://oeis.org/A119482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, less
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = n-th integer whose English name has letter sum < n. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* Letter sums for number names (a=1 … z=26). */
static const uint8_t sum_units[20] = {
    64,  /* zero  */
    34,  /* one   */
    58,  /* two   */
    56,  /* three */
    60,  /* four  */
    42,  /* five  */
    52,  /* six   */
    65,  /* seven */
    49,  /* eight */
    42,  /* nine  */
    39,  /* ten   */
    63,  /* eleven */
    87,  /* twelve */
    99,  /* thirteen */
    104, /* fourteen */
    81,  /* fifteen */
    96,  /* sixteen */
    109, /* seventeen */
    73,  /* eighteen */
    86   /* nineteen */
};

static const uint8_t sum_tens[10] = {
    0,    /* 0 */
    0,    /* 1 */
    107,  /* twenty  */
    100,  /* thirty  */
    84,   /* forty   */
    66,   /* fifty   */
    97,   /* sixty   */
    110,  /* seventy */
    74,   /* eighty  */
    87    /* ninety  */
};

static const uint8_t sum_hundred  = 74;  /* hundred */
static const uint8_t sum_thousand = 102; /* thousand */
static const uint8_t sum_million  = 84;  /* million */
static const uint8_t sum_billion  = 73;  /* billion */

static uint64_t _letter_sum(uint64_t n)
{
    uint64_t sum = 0;

    if (n >= 1000000000ULL) {
        uint64_t bn = n / 1000000000ULL;
        sum += _letter_sum(bn) + sum_billion;
        n %= 1000000000ULL;
        if (n == 0) return sum;
    }

    if (n >= 1000000ULL) {
        uint64_t mn = n / 1000000ULL;
        sum += _letter_sum(mn) + sum_million;
        n %= 1000000ULL;
        if (n == 0) return sum;
    }

    if (n >= 1000ULL) {
        uint64_t tn = n / 1000ULL;
        sum += _letter_sum(tn) + sum_thousand;
        n %= 1000ULL;
        if (n == 0) return sum;
    }

    if (n >= 100ULL) {
        uint64_t hn = n / 100ULL;
        sum += _letter_sum(hn) + sum_hundred;
        n %= 100ULL;
        if (n == 0) return sum;
    }

    if (n >= 20ULL) {
        uint64_t td = n / 10ULL;   /* 2 … 9 */
        sum += sum_tens[td];
        n %= 10ULL;
        if (n == 0) return sum;
    }

    /* n < 20 */
    sum += sum_units[(int)n];
    return sum;
}

/* Static storage for memoisation. */
static seq_t   *terms = NULL; /* array of found terms */
static size_t   len   = 0;    /* number of stored terms   */
static size_t   cap   = 0;    /* current allocation size */
static uint64_t next  = 0;    /* next integer to test     */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (cap == 0) {
        cap   = 16;
        terms = seq_alloc_array(cap);
    }

    while (len < (size_t)n) {
        ++next;                       /* test this number */
        uint64_t sum = _letter_sum(next);
        if (sum < next) {
            if (len == cap) {
                size_t new_cap = cap << 1;
                seq_t *new_terms = seq_alloc_array(new_cap);
                for (size_t i = 0; i < len; ++i) new_terms[i] = terms[i];
                seq_free_array(terms);
                terms = new_terms;
                cap   = new_cap;
            }
            terms[len++] = (seq_t)next;
        }
    }

    return terms[n - 1];
}
