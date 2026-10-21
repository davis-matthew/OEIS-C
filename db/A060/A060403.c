/*
 * A060403  Each term is the previous term plus the number of letters in the
 *          previous number, as conventionally spelled out in American
 *          English.
 * https://oeis.org/A060403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1964, u32 n <= 9551, u64 n <= 9551,
 *            u128 n <= 9551, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = a(n‑1) + L(a(n‑1))
   where L(x) is the number of letters in the American English spelling of x
   (no “and”). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const unsigned int unit_len[10]  = {4,3,3,5,4,4,3,5,5,4}; /* zero‑nine */
static const unsigned int teen_len[10] = {3,6,6,8,8,7,7,9,8,8}; /* ten‑nineteen*/
static const unsigned int tens_len[10] = {0,0,6,6,5,5,5,7,6,6}; /* 0-90 tens */
static const unsigned int hundred_len = 7;          /* “hundred” */
static const unsigned int group_name_len[4] = {8,7,7,8}; /* thousand,million, ... */

/* length of a 1‑to‑3‑digit group in words */
static unsigned int group_letters(unsigned int g)
{
    unsigned int len = 0;
    if (g >= 100) {
        unsigned int h = g / 100;
        len += unit_len[h] + hundred_len;
        g %= 100;
        if (g == 0) return len;
    }
    if (g >= 20) {
        unsigned int t = g / 10;
        len += tens_len[t];
        g %= 10;
        if (g == 0) return len;
    }
    if (g >= 10) {          /* 10‑19 */
        len += teen_len[g - 10];
        return len;
    }
    if (g > 0)
        len += unit_len[g];
    return len;
}

/* total number of letters for a non‑negative integer n */
static unsigned int letters_count(uint64_t n)
{
    if (n == 0) return 4; /* “zero” */
    unsigned int total = 0;
    unsigned int group_idx = 0;   /* 0 = units, 1 = thousand, … */
    while (n > 0) {
        unsigned int group = (unsigned int)(n % 1000);
        if (group > 0) {
            unsigned int len = group_letters(group);
            if (group_idx > 0 && group_idx <= 4)
                len += group_name_len[group_idx - 1];
            total += len;
        }
        n /= 1000;
        ++group_idx;
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */
    seq_t val = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        unsigned int len = letters_count((uint64_t)val);
        val += (seq_t)len;
    }
    return val;
}
