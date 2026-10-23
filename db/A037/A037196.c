/*
 * A037196  Number of vowels in the American English name of n.
 * https://oeis.org/A037196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of vowels (a, e, i, o, u) in the American English name of n. */
/* Uses recursion over groups of three digits; no external libraries needed. */

#include <stdint.h>

/* Vowel count for the words 0–19. */
static const int unit_vowel[20] = {
    2, /* 0 zero   */
    2, /* 1 one    */
    1, /* 2 two    */
    2, /* 3 three  */
    2, /* 4 four   */
    2, /* 5 five   */
    1, /* 6 six    */
    2, /* 7 seven  */
    2, /* 8 eight  */
    2, /* 9 nine   */
    1, /*10 ten    */
    3, /*11 eleven */
    2, /*12 twelve */
    3, /*13 thirteen */
    4, /*14 fourteen */
    3, /*15 fifteen */
    3, /*16 sixteen */
    4, /*17 seventeen */
    4, /*18 eighteen */
    4  /*19 nineteen */
};

/* Vowel count for the tens words 20,30,…,90 (index 0,1 unused). */
static const int tens_vowel[10] = {
    0, /* 0 */
    0, /* 1 */
    1, /* 2 twenty */
    1, /* 3 thirty */
    1, /* 4 forty  */
    1, /* 5 fifty  */
    1, /* 6 sixty  */
    2, /* 7 seventy */
    2, /* 8 eighty  */
    2  /* 9 ninety  */
};

/* Vowel count for scale names: thousand, million, billion, ... duodecillion. */
static const int scale_vowel[20] = {
    0, /* index 0 unused */
    3, /* 1 thousand */
    3, /* 2 million */
    3, /* 3 billion */
    3, /* 4 trillion */
    5, /* 5 quadrillion */
    5, /* 6 quintillion */
    5, /* 7 sextillion */
    5, /* 8 septillion */
    5, /* 9 octillion */
    4, /*10 nonillion */
    4, /*11 decillion */
    5, /*12 undecillion */
    6  /*13 duodecillion */
};

/* Count vowels in the name of a three‑digit block (1 .. 999). */
static inline seq_t count_block(int g)
{
    seq_t cnt = 0;
    if (g >= 100) {
        int h = g / 100;            /* hundreds digit 1 .. 9 */
        cnt += (seq_t)unit_vowel[h];
        cnt += (seq_t)2;            /* "hundred" has 2 vowels */
        g %= 100;
        if (g == 0) return cnt;
    }
    if (g >= 20) {
        int t = g / 10;            /* tens digit 2 .. 9 */
        cnt += (seq_t)tens_vowel[t];
        g %= 10;
        if (g == 0) return cnt;
    }
    if (g > 0) {
        cnt += (seq_t)unit_vowel[g];
    }
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)unit_vowel[0];   /* "zero" */
    seq_t total = 0;
    idx_t temp = n;
    int group_index = 0;
    while (temp > 0) {
        int group_val = (int)(temp % 1000);
        temp /= 1000;
        if (group_val != 0) {
            total += count_block(group_val);
            if (group_index > 0) {
                total += (seq_t)scale_vowel[group_index];
            }
        }
        group_index++;
    }
    return total;
}
