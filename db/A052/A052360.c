/*
 * A052360  Number of characters in the English name of n, including spaces
 *          and hyphens.
 * https://oeis.org/A052360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, nice, easy
 * Formula:   a(n) = number of characters (letters, spaces, hyphens) in the
 *            English full name of n, including spaces and hyphens. The
 *            function expands n into groups of three digits and sums the
 *            corresponding word lengths
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 8995 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Length of the English name of a number 1–99 (x > 0).  */
static seq_t len_1_99(idx_t num)
{
    /* lengths of the unit words (0 unused) */
    static const uint32_t unit_len[10]   = {0, 3, 3, 5, 4, 4, 3, 5, 5, 4};
    /* lengths of the teen words 10–19 */
    static const uint32_t teen_len[10]   = {3, 6, 6, 8, 8, 7, 7, 9, 8, 8};
    /* lengths of the tens words 20,30,…,90 */
    static const uint32_t tens_len[10]   = {0, 0, 6, 6, 5, 5, 5, 7, 6, 6};

    if (num < 10)
        return (seq_t)unit_len[num];
    if (num < 20)
        return (seq_t)teen_len[num - 10];

    /* 20–99 */
    int tens  = num / 10;
    int unit  = num % 10;
    seq_t len = (seq_t)tens_len[tens];
    if (unit)
        len += (seq_t)1 + (seq_t)unit_len[unit];   /* hyphen + unit word */
    return len;
}

/* Length of the English name of a number 1–999 (x > 0).  */
static seq_t len_1_999(idx_t num)
{
    /* lengths of the unit words (0 unused) */
    static const uint32_t unit_len[10] = {0, 3, 3, 5, 4, 4, 3, 5, 5, 4};

    seq_t total = 0;
    int h   = num / 100;       /* hundreds digit */
    int rem = num % 100;       /* remainder */

    if (h) {
        total += (seq_t)unit_len[h] + (seq_t)1 + (seq_t)7; /* unit + space + "hundred" */
        if (rem)
            total += (seq_t)1 + len_1_99(rem);           /* space + remainder */
    } else if (rem) {
        total += len_1_99(rem);
    }
    return total;
}

 

/* Main sequence generator.  */
seq_t a(idx_t n)
{
    /* Special case for zero. */
    if (n == 0)
        return (seq_t)4;  /* "zero" */

    /* Factor n into groups of three digits: units, thousands, millions, … */
    const int MAX_GROUPS = 20;          /* enough for 10^60+ */
    idx_t groups[MAX_GROUPS];
    int n_groups = 0;
    idx_t tmp = n;
    while (tmp && n_groups < MAX_GROUPS) {
        groups[n_groups++] = tmp % 1000;
        tmp /= 1000;
    }

    /* Lengths of the scale words for groups 1..19.  */
    static const uint32_t scale_len[20] = {
        0,    /* unused for 10^0 */
        8,    /* thousand  10^3  */
        7,    /* million   10^6  */
        7,    /* billion   10^9  */
        8,    /* trillion  10^12 */
        10,   /* quadrillion 10^15 */
        10,   /* quintillion 10^18 */
        9,    /* sextillion 10^21 */
        9,    /* septillion 10^24 */
        8,    /* octillion 10^27 */
        9,    /* nonillion 10^30 */
        8,    /* decillion 10^33 */
        0,0,0,0,0,0,0   /* unused beyond 10^33 in this build */
    };

    /* lower_nonzero[i] = true if any group j < i is non‑zero. */
    bool lower_nonzero[MAX_GROUPS];
    lower_nonzero[0] = false;
    for (int i = 1; i < n_groups; ++i)
        lower_nonzero[i] = lower_nonzero[i - 1] || (groups[i - 1] != 0);

    seq_t total = 0;
    for (int i = n_groups - 1; i >= 0; --i) {
        if (groups[i] == 0)
            continue;

        /* Add the words for this group (hundreds, tens, units). */
        total += len_1_999(groups[i]);

        /* Add the scale word for groups >= 1. */
        if (i > 0) {
            total += (seq_t)1 + (seq_t)scale_len[i];   /* space + scale word */
            if (lower_nonzero[i])
                total += (seq_t)1;                    /* space before next group */
        }
    }
    return total;
}
