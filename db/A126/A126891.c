/*
 * A126891  Number of letters in the English name of n, excluding spaces and
 *          hyphens and all integers preceding it.
 * https://oeis.org/A126891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is the partial sum of the letter counts of the
   English names of the non‑negative integers.
   Formally:
      a(n) = Σ_{k=0..n} letters_of(k)
   where
      letters_of(k) = number of letters in the English name of k,
                      excluding spaces and hyphens. */
#include <stddef.h>   /* for size_t */

/* Letter counts for units (0–19). */
static const unsigned char unit_len[20] = {
    4, /* zero  */
    3, /* one   */
    3, /* two   */
    5, /* three */
    4, /* four  */
    4, /* five  */
    3, /* six   */
    5, /* seven */
    5, /* eight */
    4, /* nine  */
    3, /* ten   */
    6, /* eleven*/
    6, /* twelve*/
    8, /* thirteen*/
    8, /* fourteen*/
    7, /* fifteen*/
    7, /* sixteen*/
    9, /* seventeen*/
    8, /* eighteen*/
    8  /* nineteen*/
};

/* Letter counts for tens (20–90). */
static const unsigned char tens_len[10] = {
    0, 0, /* 0–9 unused */
    6, /* twenty */
    6, /* thirty */
    5, /* forty  */
    5, /* fifty  */
    5, /* sixty  */
    7, /* seventy*/
    6, /* eighty */
    6  /* ninety */
};

/* Length of the word "hundred" */
static const unsigned char len_hundred = 7;

/* Scale values and their word lengths */
static const idx_t  scale_vals[] = {
    1000000000000ULL, /* trillion */
    1000000000ULL,    /* billion */
    1000000ULL,       /* million */
    1000ULL           /* thousand */
};
static const unsigned char scale_len[] = {9, 7, 7, 8}; /* lengths of the words above */
static const size_t SCALE_COUNT = sizeof(scale_vals)/sizeof(scale_vals[0]);

/* Compute letters for a number < 1000 (1..999). For 0 the result is 0. */
static inline seq_t letters_upto_999(idx_t m)
{
    if (m == 0)
        return 0;

    seq_t res = 0;

    if (m >= 100) {
        idx_t h = m / 100;
        m   %= 100;
        res += (seq_t)unit_len[h] + len_hundred;   /* e.g., "three hundred" */
        if (m == 0)
            return res;   /* no remainder */
    }

    if (m >= 20) {
        idx_t t = m / 10;
        m   %= 10;
        res += tens_len[t];
    }

    if (m > 0) {          /* for 1–19 or units after the tens */
        res += unit_len[m];
    }

    return res;
}

/* Compute number of letters in the English name of a single integer. */
static inline seq_t letters_of(idx_t n)
{
    if (n == 0)
        return unit_len[0];   /* "zero" */

    seq_t len = 0;

    /* Handle each scale from largest to smallest */
    for (size_t i = 0; i < SCALE_COUNT; ++i) {
        if (n >= scale_vals[i]) {
            idx_t grp = n / scale_vals[i];
            n -= grp * scale_vals[i];
            len += letters_upto_999(grp) + scale_len[i];
        }
    }

    /* Handle the units part (<1000) */
    len += letters_upto_999(n);
    return len;
}

/* Main sequence function: partial sums of letter counts. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        sum += letters_of(i);
    }
    return sum;
}
