/*
 * A192740  Spanish version of A060403 - Each term is the previous term plus
 *          the number of letters in the previous number, as conventionally
 *          spelled out in Spanish.
 * https://oeis.org/A192740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(2) = L(1);  a(n) = a(n-1) + L(a(n-1)) for n ≥ 3
   L(x) is the number of letters in the conventionally spelled‑out Spanish
   word for x (spaces and hyphens ignored). */

#include <stdint.h>

/* Basic lengths of Spanish number words (only letters). */
static const unsigned char unit_len[10]  = {0,3,3,4,6,5,4,5,4,5};
static const unsigned char teen_len[10]  = {4,4,4,5,7,6,9,10,9,10};
static const unsigned char tens_len[10]  = {0,0,0,7,8,9,7,7,7,7}; /* 30,40,…,90 */
static const unsigned char hundred_name[10] =
               {0,0,10,11,13,10,11,11,11,11};                /* 200,300,…,900 */

/* Return the number of letters of the Spanish name of n (n > 0). */
static unsigned int letter_len(seq_t n)
{
    /* 1–9 */
    if (n < 10)
        return unit_len[(int)n];

    /* 10–19 */
    if (n < 20)
        return teen_len[(int)(n-10)];

    /* 20–29 */
    if (n < 30)
        return 6 + unit_len[(int)(n-20)];   /* "veinte" + unit */

    /* 30–99 */
    if (n < 100) {
        int tens  = (int)((n / 10) % 10);
        int unit  = (int)(n % 10);
        if (unit == 0)
            return tens_len[tens];
        return tens_len[tens] + 1 + unit_len[unit];  /* +1 for "y" */
    }

    /* 100–999 */
    if (n < 1000) {
        int hundreds = (int)(n / 100);
        int rest = (int)(n % 100);
        if (hundreds == 1) {          /* 100–199 */
            if (rest == 0)
                return 4;             /* "cien" */
            return 6 + letter_len((seq_t)rest); /* "ciento" + rest */
        }
        int len = hundred_name[hundreds];
        if (rest == 0)
            return len;
        return len + letter_len((seq_t)rest);
    }

    /* 1 000–999 999 */
    if (n < 1000000) {
        seq_t thousands = n / 1000;
        seq_t rest = n % 1000;
        if (thousands == 1) {         /* "mil" */
            if (rest == 0)
                return 3;
            return 3 + letter_len(rest);
        }
        int len = letter_len(thousands) + 3;  /* "mil" */
        if (rest == 0)
            return len;
        return len + letter_len(rest);
    }

    /* 1 000 000–999 999 999 */
    if (n < 1000000000) {
        seq_t millions = n / 1000000;
        seq_t rest = n % 1000000;
        if (millions == 1) {          /* "un" + "millon" */
            int len = 2 + 6;          /* 2 ("un") + 6 ("millon") */
            if (rest == 0)
                return len;
            return len + letter_len(rest);
        }
        int len = letter_len(millions) + 8; /* "millones" */
        if (rest == 0)
            return len;
        return len + letter_len(rest);
    }

    /* Numbers ≥ 1 000 000 000 use a generic fallback. */
    if (n >= 1000000000) {
        seq_t billions = n / 1000000000;
        seq_t rest = n % 1000000000;
        int len = letter_len(billions) + 3;       /* generic "mil" */
        if (rest == 0)
            return len;
        return len + letter_len(rest);
    }

    return 0; /* unreachable */
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if (n == 2)
        return (seq_t)letter_len((seq_t)1);   /* L(1) */

    seq_t cur = (seq_t)3; /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
        cur += (seq_t)letter_len(cur);
    return cur;
}
