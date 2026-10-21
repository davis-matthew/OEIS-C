/*
 * A051785  Number of letters in n-th Catalan number.
 * https://oeis.org/A051785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, word, nice
 * Formula:   a(n) = number of letters in the Catalan word for n
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const unsigned char digit_len[10] = {
    0, /* 0 unused */
    2, /* un   */
    3, /* dos  */
    4, /* tres */
    6, /* quatre */
    4, /* cinc  */
    3, /* sis   */
    3, /* set   */
    4, /* vuit  */
    3  /* nou   */
};

static const unsigned char teen_len[10] = {
    3, /* deu   */
    4, /* onze  */
    5, /* dotze */
    6, /* tretze*/
    7, /* catorze*/
    6, /* quinze*/
    5, /* setze */
    6, /* disset*/
    6, /* divuit*/
    5  /* dinou */
};

static const unsigned char tens_len[10] = {
    0, /* 0 unused */
    0, /* 10 handled above */
    4, /* vint  */
    6, /* trenta */
    8, /* quaranta */
    9, /* cinquanta */
    8, /* seixanta */
    7, /* setanta */
    8, /* vuitanta */
    7  /* noranta */
};

/* Recursive helper that returns the letter count of the Catalan word for n. */
static seq_t catlen(idx_t n)
{
    if (n < 10) {
        return digit_len[(unsigned)n];
    }

    if (n < 20) {
        return teen_len[(unsigned)(n - 10)];
    }

    if (n < 100) {
        unsigned tens = (unsigned)(n / 10);
        unsigned units = (unsigned)(n % 10);
        if (units == 0) {
            return tens_len[tens];
        }
        /* For tens 2 use the conjunction 'i', for tens >=3 omit it. */
        if (tens == 2) {
            return tens_len[tens] + 1 + digit_len[units];
        }
        return tens_len[tens] + digit_len[units];
    }

    if (n < 1000) {
        unsigned hundreds = (unsigned)(n / 100);
        unsigned rest = (unsigned)(n % 100);

        /* Exact hundreds. */
        if (rest == 0) {
            if (hundreds == 1) {          /* 100 */
                return 4;                 /* 'cent' */
            } else {                      /* 200‑900 */
                return digit_len[hundreds] + 5; /* digit + 'cents' */
            }
        }

        /* Hundreds with remainder. */
        seq_t part;
        if (hundreds == 1) {                /* 100‑199 */
            part = 4;                       /* 'cent' */
        } else {                            /* 200‑999 */
            part = digit_len[hundreds] + 5; /* digit + 'cents' */
        }
        return part + catlen(rest);
    }

    /* n >= 1000: thousands group.  */
    idx_t thousands = n / 1000;
    idx_t rest = n % 1000;
    seq_t th_len;
    if (thousands == 1) {
        th_len = 3;                 /* "mil" */
    } else {
        th_len = catlen(thousands) + 3; /* number + "mil" */
    }
    seq_t rest_len = rest ? catlen(rest) : 0;
    return th_len + rest_len;
}

seq_t a(idx_t n)
{
    return catlen(n);
}
