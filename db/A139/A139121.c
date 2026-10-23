/*
 * A139121  Total number of letters in the preceding terms spelled out in
 *          French.
 * https://oeis.org/A139121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(1) = 0 a(n) = a(n‑1) + L(a(n‑1)) for n ≥ 2 where L(x) is the
 *            number of letters in the French spelling of x (hyphens, spaces
 *            and accents are ignored)
 * Range:     u8 n <= 22, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------- Basic lengths of French words (letters only) ------- */
static const seq_t LEN_UNIT[17] = {
    4, /* 0  "zero"    */
    2, /* 1  "un"      */
    4, /* 2  "deux"    */
    5, /* 3  "trois"   */
    6, /* 4  "quatre"  */
    4, /* 5  "cinq"    */
    3, /* 6  "six"     */
    4, /* 7  "sept"    */
    4, /* 8  "huit"    */
    4, /* 9  "neuf"    */
    3, /*10  "dix"     */
    4, /*11  "onze"    */
    5, /*12  "douze"   */
    6, /*13  "treize"  */
    8, /*14  "quatorze"*/
    6, /*15  "quinze"  */
    5  /*16  "seize"   */
};

static const seq_t LEN_TENS[7] = {0,0,5,6,8,9,8};  /* 20:5,30:6,40:8,50:9,60:8 */

/* ------- Length of the French spelling of a number < 1 000 000 -------
   (hyphens, spaces and accents are ignored).  The result fits easily
   into a seq_t small value (less than 20).                */
static seq_t french_len_uint64(uint64_t n)
{
    /* 0 – "zero" */
    if (n == 0) return 4;

    /* 1 … 16 */
    if (n <= 16) return LEN_UNIT[n];

    /* 17 … 19 : "dix-" + unit */
    if (n <= 19) return LEN_UNIT[10] + LEN_UNIT[n - 10];

    /* 20 … 69 */
    if (n <= 69) {
        uint64_t tens = n / 10;
        uint64_t unit = n % 10;
        seq_t len = LEN_TENS[tens];
        if (unit == 0) return len;
        if (unit == 1)         return len + 2 + LEN_UNIT[1]; /* "et" + "un" */
        return len + LEN_UNIT[unit];
    }

    /* 70 … 79 : "soixante-" + (10 … 19) */
    if (n <= 79) {
        uint64_t rem = n % 10;
        if (rem == 0)         return 8 + LEN_UNIT[10];       /* 70 */
        if (rem == 1)         return 8 + 2 + LEN_UNIT[11];  /* 71 */
        return 8 + french_len_uint64(10 + rem);                 /* 72‑79 */
    }

    /* 80 … 99 */
    if (n <= 99) {
        if (n == 80)              return 12;                  /* "quatre-vingts" */
        uint64_t rem = n % 10;
        if (n < 90)               return 6 + 5 + LEN_UNIT[rem];        /* 81‑89 */
        return 6 + 5 + french_len_uint64(10 + rem);                 /* 90‑99 */
    }

    /* 100 … 999 */
    if (n <= 999) {
        uint64_t hund = n / 100;
        uint64_t rem  = n % 100;
        if (rem == 0) {
            if (hund == 1)        return 4;      /* "cent"  */
            else                 return LEN_UNIT[hund] + 5; /* "cents" */
        } else {
            seq_t len_rem = french_len_uint64(rem);
            if (hund == 1)        return 4 + len_rem;                 /* 101‑199 */
            else                 return LEN_UNIT[hund] + 4 + len_rem; /* 200‑999 */
        }
    }

    /* 1 000 … 999 999 : thousands + remainder */
    uint64_t thousand = n / 1000;
    uint64_t rem = n % 1000;
    seq_t tlen;

    if (thousand == 1) {
        tlen = 5;                         /* "mille" */
    } else {
        tlen = french_len_uint64(thousand);           /* normal spelling */
        if (thousand >= 200 && thousand % 100 == 0)
            tlen -= 1;                    /* remove plural 's' on "cent" */
        tlen += 5;                        /* add "mille" */
    }

    if (rem == 0) return tlen;
    return tlen + french_len_uint64(rem);
}

/* ------- Sequence generator ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;               /* a(1) = 0 */

    seq_t cur = 0;                      /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t l = french_len_uint64((uint64_t)cur); /* len of a(i-1) */
        cur += l;
    }
    return cur;
}
