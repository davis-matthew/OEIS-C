/*
 * A052362  Indices of records in length of English name of n including spaces
 *          and dashes (A052360): n such that k < n => A052360(k) <
 *          A052360(n).
 * https://oeis.org/A052362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A052362 – Indices of records in length of English name
 *  (including spaces and dashes).  This implementation generates
 *  numbers in order, computing the length of their English name
 *  (US style, no “and”) and keeps a running maximum.  The next
 *  index n returns the n‑th record.
 *
 *  The algorithm is a simple brute‑force search; the reasonably
 *  small density of records allows all terms needed for n≤10 000
 *  to be generated quickly.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper tables ------------------------------------------------------- */

/* Lengths of the words “zero” … “nineteen”. */
static const int len_units[20] = {
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

/* Lengths of the words “twenty” … “ninety”.  Index 0 and 1 are unused. */
static const int len_tens[10] = {
    0, 0, 6, /* twenty */
    6, /* thirty */
    5, /* forty  */
    5, /* fifty  */
    5, /* sixty  */
    7, /* seventy*/
    6, /* eighty */
    6  /* ninety */
};

/* Length of the word “hundred”. */
static const int len_hundred = 7;

/* Lengths of scale words for each group of three digits.
 * Index equals the power of 1000 (0 – no scale, 1 – thousand, …). */
static const int scale_len[] = {
    0,  /* none      */
    8,  /* thousand  */
    7,  /* million   */
    7,  /* billion   */
    8,  /* trillion  */
    11, /* quadrillion */
    11, /* quintillion */
    10, /* sextillion   */
    10, /* septillion   */
    9,  /* octillion    */
    10, /* nonillion    */
    9   /* decillion    */
};

/* Utility to compute the length of a three‑digit number 1 … 999. */
static seq_t
len_three(uint64_t n)
{
    seq_t len = 0;
    uint64_t h = n / 100;
    uint64_t r = n % 100;

    if (h > 0) {
        len += len_units[h];   /* e.g. “one” */
        len += 1;              /* space     */
        len += len_hundred;    /* “hundred” */
        if (r > 0)
            len += 1;          /* space after “hundred” */
    }

    if (r > 0) {
        if (r < 20) {
            len += len_units[r];
        } else {
            uint64_t t = r / 10;   /* tens digit */
            uint64_t u = r % 10;   /* unit digit */
            len += len_tens[t];
            if (u > 0) {
                len += 1;           /* hyphen */
                len += len_units[u];
            }
        }
    }

    return len;
}

/* Computes the length of the English name of a non‑negative integer,
 * counting spaces and hyphens.  Zero is handled specially. */
static seq_t
name_len(uint64_t n)
{
    if (n == 0)
        return 4;   /* “zero” */

    uint64_t groups[20];   /* 3‑digit groups, 20 is ample for 64‑bit numbers */
    int gcount = 0;
    uint64_t tmp = n;

    while (tmp > 0) {
        groups[gcount++] = tmp % 1000;
        tmp /= 1000;
    }

    seq_t len = 0;
    int first = 1;

    for (int i = gcount - 1; i >= 0; i--) {
        uint64_t g = groups[i];
        if (g == 0)
            continue;

        if (!first) {
            len += 1;   /* space between groups */
        }

        /* Words for the 3‑digit group */
        len += len_three(g);

        if (i > 0) {
            len += 1;                  /* space before the scale word */
            len += scale_len[i];
        }
        first = 0;
    }

    return len;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Static cache of discovered record indices */
    static seq_t *records = NULL;
    static size_t records_len = 0;
    static size_t records_cap = 0;
    static idx_t next_num = 0;
    static seq_t curr_max_len = 0;

    /* Initialise once */
    if (!records) {
        records_cap = 128;
        records = (seq_t *)malloc(records_cap * sizeof(seq_t));
        next_num = 0;
        curr_max_len = 0;
    }

    /* Satisfy the requested number of records */
    while (records_len < n) {
        seq_t len = name_len((uint64_t)next_num);

        if (len > curr_max_len) {
            /* Store the new record index */
            if (records_len == records_cap) {
                records_cap *= 2;
                records = (seq_t *)realloc(records, records_cap * sizeof(seq_t));
            }
            records[records_len++] = (seq_t)next_num;
            curr_max_len = len;
        }
        next_num++;
    }

    return records[n - 1];
}
