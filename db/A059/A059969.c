/*
 * A059969  a(n) = n times the number of letters in n (in English).
 * https://oeis.org/A059969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = n*A005589(n)
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* lengths of words for numbers 0–19 */
    static const int under20_len[20] = {
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

    /* lengths of words for tens 20–90 */
    static const int tens_len[10] = {
        0, /* unused */
        0, /* unused */
        6, /* twenty */
        6, /* thirty */
        5, /* forty  */
        5, /* fifty  */
        5, /* sixty  */
        7, /* seventy*/
        6, /* eighty */
        6  /* ninety */
    };

    /* lengths of scale words: thousand, million, billion, trillion,
       quadrillion, quintillion (index 0 unused) */
    static const int scale_len[7] = {
        0,  /* none   */
        8,  /* thousand */
        7,  /* million  */
        7,  /* billion  */
        8,  /* trillion */
        11, /* quadrillion */
        11  /* quintillion */
    };

    seq_t total_len = 0;
    idx_t m = n;
    int scale_index = 0;

    while (m > 0)
    {
        idx_t block = m % 1000;          /* current 3‑digit block */
        if (block > 0)
        {
            seq_t block_len = 0;
            int h = block / 100;         /* hundreds digit */
            int t = (block / 10) % 10;   /* tens digit   */
            int u = block % 10;          /* units digit  */

            if (h > 0)
            {
                block_len += (seq_t)under20_len[h] + 7; /* "hundred" */
            }

            int rem = block % 100;
            if (rem < 20)
            {
                if (rem > 0)
                    block_len += (seq_t)under20_len[rem];
            }
            else
            {
                int tens_digit = rem / 10;
                int units_digit = rem % 10;
                block_len += (seq_t)tens_len[tens_digit];
                if (units_digit > 0)
                    block_len += (seq_t)under20_len[units_digit];
            }

            /* add scale word length if needed */
            if (scale_index > 0)
                block_len += (seq_t)scale_len[scale_index];

            total_len += block_len;
        }

        m /= 1000;
        scale_index++;
    }

    return (seq_t)n * total_len;
}
