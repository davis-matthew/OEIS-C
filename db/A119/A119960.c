/*
 * A119960  Positive integers with prime number of characters in their English
 *          names, including spaces and hyphens.
 * https://oeis.org/A119960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = the n-th positive integer whose English name, counting
 *            letters, spaces and hyphens, has a prime number of characters
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    The algorithm searches forward from 1, computing the
   character length of each integer's English name and keeping
   those whose length is prime.  The results are cached so
   subsequent calls, even out of order, return in O(1).  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper tables ---------------------------------------------- */
static const int ones_len[20] = {
    4, /* 0: "zero" */
    3, /* 1: "one"  */
    3, /* 2: "two"  */
    5, /* 3: "three"*/
    4, /* 4: "four" */
    4, /* 5: "five" */
    3, /* 6: "six"  */
    5, /* 7: "seven"*/
    5, /* 8: "eight"*/
    4, /* 9: "nine" */
    3, /*10: "ten"  */
    6, /*11: "eleven"*/
    6, /*12: "twelve"*/
    8, /*13: "thirteen"*/
    8, /*14: "fourteen"*/
    7, /*15: "fifteen"*/
    7, /*16: "sixteen"*/
    9, /*17: "seventeen"*/
    8, /*18: "eighteen"*/
    8  /*19: "nineteen"*/
};

static const int tens_len[10] = {
    0, /*0 */
    0, /*1 */
    6, /*2: "twenty"*/
    6, /*3: "thirty"*/
    5, /*4: "forty"*/
    5, /*5: "fifty"*/
    5, /*6: "sixty"*/
    7, /*7: "seventy"*/
    6, /*8: "eighty"*/
    6  /*9: "ninety"*/
};

static const int scale_len[10] = {
    8,  /* thousand  */
    7,  /* million   */
    7,  /* billion   */
    8,  /* trillion  */
    11, /* quadrillion */
    10, /* quintillion */
    9,  /* sextillion  */
    9,  /* septillion  */
    8,  /* octillion   */
    9,  /* nonillion   */
    /* we stop here; higher scales not needed for n ≤ 10000 */
};

/* Primitive prime test for small lengths */
static bool is_prime_len(int len) {
    if (len < 2) return false;
    for (int d = 2; d * d <= len; ++d)
        if (len % d == 0) return false;
    return true;
}

/* Compute the character length of the English name of n
   (including spaces and hyphens, but not a trailing space). */
static int eng_len(uint64_t n) {
    if (n == 0) return ones_len[0];       /* "zero" */

    uint64_t tmp = n;
    int groups[10] = {0};
    int group_cnt = 0;
    while (tmp > 0 && group_cnt < 10) {
        groups[group_cnt++] = tmp % 1000;
        tmp /= 1000;
    }

    int total_len = 0;
    bool first_group = true;

    for (int idx = group_cnt - 1; idx >= 0; --idx) {
        int g = groups[idx];
        if (g == 0) continue;

        if (!first_group) total_len += 1;    /* space before this group */
        int len = 0;

        /* hundreds part */
        if (g >= 100) {
            int h = g / 100;
            len += ones_len[h];
            len += 7; /* "hundred" */
            int rem = g % 100;
            if (rem > 0) {
                len += 1; /* space after hundred */
                if (rem < 20) {
                    len += ones_len[rem];
                } else {
                    int t = rem / 10;
                    int u = rem % 10;
                    len += tens_len[t];
                    if (u > 0) {
                        len += 1;          /* hyphen */
                        len += ones_len[u];
                    }
                }
            }
        } else { /* 0-99 */
            if (g < 20) {
                len += ones_len[g];
            } else {
                int t = g / 10;
                int u = g % 10;
                len += tens_len[t];
                if (u > 0) {
                    len += 1;          /* hyphen */
                    len += ones_len[u];
                }
            }
        }

        /* scale part (thousand, million, ...), if any */
        if (idx > 0) {
            len += 1;                    /* space before scale name */
            len += scale_len[idx - 1];
        }

        total_len += len;
        first_group = false;
    }

    return total_len;
}

/* ------------------------------------------------------------- */
/*  Sequence implementation ------------------------------------- */

#ifdef __cplusplus
#include <vector>
#endif

/* External linkage as requested */
seq_t a(idx_t n) {
    /* static state shared between calls */
    static idx_t last_checked = 0;               /* last number examined */

#ifdef __cplusplus
    static std::vector<seq_t> cache;             /* pooled results */
#else
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
#endif

    /* Grow the cache until we have n terms */
    while (
#ifdef __cplusplus
        cache.size() < (size_t)n
#else
        cache_size < (size_t)n
#endif
    ) {
        ++last_checked;                     /* next integer to examine */

        int len = eng_len((uint64_t)last_checked);
        if (is_prime_len(len)) {
#ifdef __cplusplus
            cache.push_back((seq_t)last_checked);
#else
            /* realloc if needed */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();           /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)last_checked;
#endif
        }
    }

    /* Return the requested element */
    return
#ifdef __cplusplus
        cache[n - 1];
#else
        cache[n - 1];
#endif
}
