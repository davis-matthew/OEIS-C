/*
 * A191784  Number of e's in the English name of the n-th odd number.
 * https://oeis.org/A191784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dumb, easy, nonn, word
 * Formula:   a(n) = number of 'e's in the English name of the n-th odd number
 * Range:     u8 n <= 148, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t unit_e_counts[20] = {
    1, /* zero  */
    1, /* one   */
    0, /* two   */
    2, /* three */
    0, /* four  */
    1, /* five  */
    0, /* six   */
    2, /* seven */
    1, /* eight */
    1, /* nine  */
    1, /* ten   */
    3, /* eleven*/
    2, /* twelve*/
    2, /* thirteen */
    2, /* fourteen */
    2, /* fifteen  */
    2, /* sixteen  */
    4, /* seventeen*/
    3, /* eighteen */
    3  /* nineteen */
};

static const seq_t tens_e_counts[10] = {
    0, /* ""    */
    0, /* ""    */
    1, /* twenty */
    0, /* thirty */
    0, /* forty  */
    0, /* fifty  */
    0, /* sixty  */
    2, /* seventy */
    1, /* eighty */
    1  /* ninety */
};

static const seq_t group_e_counts[22] = {
    0,  /* unused  */
    0,  /* thousand  */
    0,  /* million   */
    0,  /* billion   */
    0,  /* trillion  */
    0,  /* quadrillion */
    0,  /* quintillion */
    1,  /* sextillion */
    1,  /* septillion */
    0,  /* octillion  */
    0,  /* nonillion  */
    1,  /* decillion  */
    1,  /* undecillion */
    1,  /* duodecillion */
    2,  /* tredecillion */
    1,  /* quattuordecillion */
    1,  /* quindecillion */
    2,  /* sexdecillion */
    3,  /* septendecillion */
    1,  /* octodecillion */
    2,  /* novemdecillion */
    0   /* vigintillion */
};

static seq_t e_count_0_to_999(uint16_t g) {
    if (g == 0) return 0;
    seq_t e = 0;

    uint16_t hd = g / 100;
    uint16_t rem = g % 100;

    if (hd != 0) {
        e += unit_e_counts[hd];
        e += 1; /* "hundred" has one 'e' */
    }

    if (rem < 20) {
        e += unit_e_counts[rem];
    } else {
        uint16_t td = rem / 10;
        uint16_t ud = rem % 10;
        e += tens_e_counts[td];
        if (ud != 0) {
            e += unit_e_counts[ud];
        }
    }

    return e;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0, return 0 for safety */

    seq_t x = (seq_t)n * 2 - 1; /* the n-th odd number */
    seq_t total = 0;
    idx_t group_index = 0;

    while (x != 0) {
        uint16_t group = (uint16_t)(x % 1000);
        if (group != 0) {
            total += e_count_0_to_999(group);
            if (group_index > 0) {
                total += group_e_counts[group_index];
            }
        }
        x /= 1000;
        group_index++;
    }
    return total;
}
