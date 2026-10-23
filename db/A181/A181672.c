/*
 * A181672  The odd terms (a1,a3,a5,...) are in an arithmetic progression
 *          (1,2,3,...), while the even terms (a2,a4,a6,...) give the number
 *          of letters of the given odd term.
 * https://oeis.org/A181672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(2n-1) = n; a(2n) = number of letters of the English name of n
 *            (hyphens and spaces are not counted)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Letter counts for the words 0–19 (index by number) */
static const int len_units[20] = {
    0, /* 0 unused */
    3, /* one     */
    3, /* two     */
    5, /* three   */
    4, /* four    */
    4, /* five    */
    3, /* six     */
    5, /* seven   */
    5, /* eight   */
    4, /* nine    */
    3, /* ten     */
    6, /* eleven  */
    6, /* twelve  */
    8, /* thirteen*/
    8, /* fourteen*/
    7, /* fifteen */
    7, /* sixteen */
    9, /* seventeen*/
    8, /* eighteen*/
    8  /* nineteen*/
};

/* Letter counts for the tens words 20,30,...,90 */
static const int len_tens[10] = {
    0, /* 0 unused */
    0, /* 10-19 handled above */
    6, /* twenty */
    6, /* thirty */
    5, /* forty  */
    5, /* fifty  */
    5, /* sixty  */
    7, /* seventy*/
    6, /* eighty */
    6  /* ninety */
};

/* Recursively count letters in the English name of n (no hyphens/spaces). */
static int letter_count(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    if (n < 20) {
        return len_units[n];
    }
    if (n < 100) {
        return len_tens[n / 10] + letter_count(n % 10);
    }
    if (n < 1000) {
        int val = len_units[n / 100] + 7;      /* "hundred" = 7 letters */
        idx_t rem = n % 100;
        if (rem) {
            val += letter_count(rem);
        }
        return val;
    }
    if (n < 1000000) {                    /* thousands */
        int val = letter_count(n / 1000) + 8; /* "thousand" = 8 letters */
        idx_t rem = n % 1000;
        if (rem) {
            val += letter_count(rem);
        }
        return val;
    }
    if (n < 1000000000) {                  /* millions */
        int val = letter_count(n / 1000000) + 7; /* "million" = 7 */
        idx_t rem = n % 1000000;
        if (rem) {
            val += letter_count(rem);
        }
        return val;
    }
    /* For completeness: billions */
    int val = letter_count(n / 1000000000) + 7; /* "billion" = 7 */
    idx_t rem = n % 1000000000;
    if (rem) {
        val += letter_count(rem);
    }
    return val;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n & 1u) {                 /* odd index: a(2n‑1) = n */
        return (seq_t)((n + 1u) / 2u);
    } else {                      /* even index: a(2n) = A005589(n) */
        return (seq_t)letter_count(n / 2u);
    }
}
