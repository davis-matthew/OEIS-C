/*
 * A075826  n minus (number of letters in English name of n).
 * https://oeis.org/A075826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, word, less
 * Formula:   a(n) = n - number_of_letters_in_english_name(n)
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: return the number of letters in the English name of n */
static seq_t letter_count_u64(idx_t n)
{
    /* lengths of the basic words */
    static const seq_t small_len[20] = {
        4, /* zero */
        3, /* one */
        3, /* two */
        5, /* three */
        4, /* four */
        4, /* five */
        3, /* six */
        5, /* seven */
        5, /* eight */
        4, /* nine */
        3, /* ten */
        6, /* eleven */
        6, /* twelve */
        8, /* thirteen */
        8, /* fourteen */
        7, /* fifteen */
        7, /* sixteen */
        9, /* seventeen */
        8, /* eighteen */
        8  /* nineteen */
    };

    static const seq_t tens_len[10] = {
        0,  /* 0 */
        0,  /* 1 */
        6,  /* twenty  */
        6,  /* thirty  */
        5,  /* forty   */
        5,  /* fifty   */
        5,  /* sixty   */
        7,  /* seventy */
        6,  /* eighty  */
        6   /* ninety  */
    };

    /* group names lengths (thousand, million, ...). index 0 means units group */
    static const seq_t group_len[10] = {
        0,  /* no name for units */
        8,  /* thousand */
        7,  /* million */
        7,  /* billion */
        8,  /* trillion */
        11, /* quadrillion */
        11, /* quintillion */
        10, /* sextillion  */
        9,  /* septillion  */
        9   /* octillion   */
    };

    if (n == 0)          /* special case for zero */
        return small_len[0]; /* "zero" = 4 letters */

    idx_t num = n;
    seq_t total = 0;
    unsigned group = 0;  /* 0 -> units, 1 -> thousand, ... */

    while (num > 0 && group < 10) {
        unsigned g = (unsigned)(num % 1000);   /* current 3‑digit group */
        num /= 1000;                           /* move to next group */

        if (g == 0) {
            ++group;
            continue;
        }

        seq_t word = 0;
        unsigned hundreds = g / 100;
        unsigned rem = g % 100;

        if (hundreds > 0) {
            word += small_len[hundreds]; /* hundreds digit */
            word += 7;                   /* "hundred" */
        }

        if (rem > 0) {
            if (rem < 20) {
                word += small_len[rem];
            } else {
                unsigned tens = rem / 10;
                unsigned units = rem % 10;
                word += tens_len[tens];
                if (units > 0)
                    word += small_len[units];
            }
        }

        /* add the group name (thousand, million, etc.) */
        word += group_len[group];

        total += word;
        ++group;
    }

    return total;
}

extern seq_t a(idx_t n)
{
    return (seq_t)n - letter_count_u64(n);
}
