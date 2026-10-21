/*
 * A085693  Let f(0) = 1, f(1) = 12, f(2) = 223, f(3) = 3334, = f(4) = 44445,
 *          f(5) = 555556, etc. Sequence gives concatenation of 0, f(0),
 *          f(f(0)), f(f(f(0))), ...
 * https://oeis.org/A085693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth digit of the concatenation of 0, f(0), f(f(0)),
 *            f(f(f(0))), ...; For a single decimal digit d (0..9), f(d) = d
 *            repeated d times followed by (d+1). For d = 9, the trailing
 *            (d+1) is the two digits 1,0. For a multi‑digit number, f(n) is
 *            obtained by concatenating f(d) for each decimal digi...
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------------------------------------------------------------------- */

#include <stdlib.h>
#include <stddef.h>

/* Patterns for f(d).  pattern_len gives the length of each row. */
static const unsigned char pattern[10][11] = {
    {1},                                   /* f(0) = 1               */
    {1,2},                                 /* f(1) = 12              */
    {2,2,3},                               /* f(2) = 223             */
    {3,3,3,4},                             /* f(3) = 3334            */
    {4,4,4,4,5},                           /* f(4) = 44445           */
    {5,5,5,5,5,6},                         /* f(5) = 555556          */
    {6,6,6,6,6,6,7},                       /* f(6) = 666667          */
    {7,7,7,7,7,7,7,8},                     /* f(7) = 7777778         */
    {8,8,8,8,8,8,8,8,9},                   /* f(8) = 88888889        */
    {9,9,9,9,9,9,9,9,9,1,0}                /* f(9) = 9999999910     */
};
static const size_t pattern_len[10] = {1,2,3,4,5,6,7,8,9,11};

static /* digits of the full sequence seen so far */
    unsigned char *seq_dig = NULL;
static size_t seq_len = 0, seq_cap = 0;

static /* digits of current g(i) (the last f applied) */
    unsigned char *cur_dig = NULL;
static size_t cur_len = 0, cur_cap = 0;

/* Ensure seq_dig has at least 'need' capacity. */
static void ensure_seq(size_t need)
{
    if (need <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap : 128;
    while (newcap < need) newcap <<= 1;
    unsigned char *p = (unsigned char *)realloc(seq_dig, newcap);
    if (!p) abort();
    seq_dig = p;
    seq_cap = newcap;
}

/* Ensure cur_dig has at least 'need' capacity. */
static void ensure_cur(size_t need)
{
    if (need <= cur_cap) return;
    size_t newcap = cur_cap ? cur_cap : 128;
    while (newcap < need) newcap <<= 1;
    unsigned char *p = (unsigned char *)realloc(cur_dig, newcap);
    if (!p) abort();
    cur_dig = p;
    cur_cap = newcap;
}

/* Append the digits of f(current) to seq_dig and replace cur_dig. */
static void iterate()
{
    /* temporary buffer for next g(i) */
    unsigned char *tmp = NULL;
    size_t tmp_len = 0, tmp_cap = 0;

    /* compute size of next */
    size_t est = cur_len * 11;                 /* worst case (d=9) */
    if (est > 0) {
        tmp_cap = est;
        tmp = (unsigned char *)malloc(tmp_cap);
        if (!tmp) abort();
    }

    /* build next */
    for (size_t i = 0; i < cur_len; ++i) {
        unsigned char d = cur_dig[i];
        const unsigned char *p = pattern[d];
        size_t plen = pattern_len[d];
        for (size_t j = 0; j < plen; ++j)
            tmp[tmp_len++] = p[j];
    }

    /* append to full sequence */
    ensure_seq(seq_len + tmp_len);
    for (size_t i = 0; i < tmp_len; ++i)
        seq_dig[seq_len + i] = tmp[i];
    seq_len += tmp_len;

    /* replace current with new */
    ensure_cur(tmp_len);
    for (size_t i = 0; i < tmp_len; ++i)
        cur_dig[i] = tmp[i];
    cur_len = tmp_len;

    free(tmp);
}

/* Public function */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (!seq_dig) {
        seq_len = 0; seq_cap = 0; seq_dig = NULL;
        cur_len = 0; cur_cap = 0; cur_dig = NULL;
        /* seed with first two terms: 0 and f(0)=1 */
        ensure_seq(2);
        seq_dig[0] = 0;       /* a(0) = 0 */
        seq_dig[1] = 1;       /* a(1) = 1 as f(0)=1 -> digit 1 */
        seq_len = 2;
        /* current g(1) = f(0) = 1 */
        ensure_cur(1);
        cur_dig[0] = 1;
        cur_len = 1;
    }

    /* extend sequence until we have index n */
    while (seq_len <= n) {
        iterate();
    }

    return (seq_t)seq_dig[n];
}
