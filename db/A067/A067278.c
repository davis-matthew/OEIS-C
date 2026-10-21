/*
 * A067278  a(n) = letters(n) + a(n-1); a(0) = 0.
 * https://oeis.org/A067278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = a(n-1) + A005589(n), a(0) = 0 (A005589(n) = number of
 *            letters in the English name of n, no hyphens or spaces)
 * Range:     u8 n <= 34, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Length of unit numbers 1..19 in letters */
static const int unit_len[20] = {
    0, /* dummy for 0 */
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

/* Length of tens 20,30,…,90 */
static const int tens_len[10] = {
    0, 0, /* 0,1 */
    6,    /* twenty */
    6,    /* thirty */
    5,    /* forty */
    5,    /* fifty */
    5,    /* sixty */
    7,    /* seventy */
    5,    /* eighty */
    6     /* ninety */
};

/* Length of scale words: thousand, million, billion … */
static const int scale_len[12] = {
    0,  /* none */
    8,  /* thousand */
    7,  /* million */
    7,  /* billion */
    8,  /* trillion */
    12, /* quadrillion */
    10, /* quintillion */
    10, /* sextillion  */
    10, /* septillion  */
    9,  /* octillion   */
    9,  /* nonillion   */
    8   /* decillion   */
};

/* Count letters in a three‑digit group (0..999), without scale word */
static seq_t letters_group(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t len = 0;
    int hundreds = (int)(n / 100);
    int rem      = (int)(n % 100);

    if (hundreds > 0) {
        len += (seq_t)unit_len[hundreds] + 7;  /* "hundred" = 7 letters */
    }

    if (rem >= 20) {
        len += (seq_t)tens_len[rem / 10];
        if (rem % 10)
            len += (seq_t)unit_len[rem % 10];
    } else if (rem > 0) {
        len += (seq_t)unit_len[rem];
    }

    return len;
}

/* Count letters in the English representation of n (no hyphens or spaces) */
static seq_t letters(idx_t n)
{
    if (n == 0)
        return 4;          /* "zero" = 4 letters, not used for a(n) */
    seq_t total = 0;
    int  scale_idx = 0;    /* 0: none, 1: thousand, 2: million, … */

    while (n > 0) {
        int group = (int)(n % 1000);
        if (group > 0) {
            total += letters_group(group);
            total += (seq_t)scale_len[scale_idx];
        }
        n /= 1000;
        ++scale_idx;
        if (scale_idx >= (int)(sizeof(scale_len) / sizeof(scale_len[0])))
            break;          /* beyond supported scales */
    }
    return total;
}

/* Public function computing the sequence term a(n) */
seq_t a(idx_t n)
{
    seq_t acc = 0;
    for (idx_t i = 1; i <= n; ++i)
        acc += letters(i);
    return acc;
}
