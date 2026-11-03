/*
 * A079741  Numbers ending with a vowel or the letter y in American English.
 * https://oeis.org/A079741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = nth number whose English name ends with a vowel or the
 *            letter y
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool is_vowel_or_y(char c)
{
    return c=='a'||c=='e'||c=='i'||c=='o'||c=='u'||c=='y';
}

/* last letters for numbers 1..19 */
static const char last1to19[20] = {
    0,  /* placeholder for 0 */
    'e','o','e','r','e','x','n','t','e', /* 1..9 */
    'n','n','e','n','n','n','n','n','n'  /*10..19*/
};

/* last letters for ones digit 0..9 */
static const char last_ones[10] = {
    0,  /* 0 */
    'e','o','e','r','e','x','n','t','e'   /*1..9*/
};

static inline bool ends_with_vowel_or_y(idx_t n)
{
    if (n == 0) return true;  /* "zero" ends with 'o' */

    while (1) {
        idx_t rem = n % 1000;
        if (rem == 0) return false;            /* multiple of 1000 → "thousand" etc. */

        if (rem < 100) {
            if (rem < 20) {
                /* 1..19 */
                return is_vowel_or_y(last1to19[(int)rem]);
            } else {
                /* 20..99 */
                idx_t ones = rem % 10;
                if (ones == 0) return true;    /* "twenty", "thirty", ... end with 'y' */
                return is_vowel_or_y(last_ones[(int)ones]);
            }
        } else { /* 100..999 */
            idx_t rest = rem % 100;
            if (rest == 0) return false;     /* ends with "hundred" */
            n = rest;                        /* examine the last two digits next */
        }
    }
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t i = 0;; ++i) {
        if (ends_with_vowel_or_y(i)) {
            if (++count == n) {
                return (seq_t)i;
            }
        }
    }
}
