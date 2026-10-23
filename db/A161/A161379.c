/*
 * A161379  Prime numbers whose Spanish names have a prime number of letters.
 * https://oeis.org/A161379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = the n‑th prime whose Spanish name has a prime number of
 *            letters
 * Range:     u8 n <= 30, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

/* Letter counts for basic Spanish words */
static const int unit_len[10]   = {0,3,3,4,6,5,4,5,4,5};        /* 0..9 */
static const int teen_len[10]   = {4,4,4,5,7,6,9,10,10,10};    /* 10..19 */
static const int tens_len[10]   = {0,0,6,7,8,9,7,7,7,7};       /* 20,30,...90 */
static const int hundred_len[10]= {0,0,10,11,13,10,11,11,11,11};/* 200..900 */

/* Count letters for numbers 0..99 */
static int count_letters_upto_99(int n)
{
    if (n < 10)
        return unit_len[n];

    if (n < 20)
        return teen_len[n - 10];

    if (n < 30) {                         /* veinte, veinticuatro, ... */
        if (n % 10 == 0)
            return 6;                     /* veinte */
        return 6 + unit_len[n % 10];
    }

    int t = n / 10;
    int u = n % 10;
    int len = tens_len[t];
    if (u != 0)
        len += 1 + unit_len[u];           /* 'y' + units */
    return len;
}

/* Count letters for numbers 0..999 */
static int count_letters_upto_999(int n)
{
    if (n < 100)
        return count_letters_upto_99(n);

    if (n == 100)
        return 4;                         /* cien */

    int h = n / 100;
    int rest = n % 100;
    int len;

    if (h == 1)                           /* ciento + rest */
        len = 6;                          /* ciento */
    else
        len = hundred_len[h];

    if (rest != 0)
        len += count_letters_upto_99(rest);

    return len;
}

/* Count letters for arbitrarily large numbers, but we only need up to ~1 000 000 for n ≤ 10 000 */
static int count_letters(uint64_t n)
{
    if (n < 1000)
        return count_letters_upto_999((int)n);

    uint64_t thousands = n / 1000;
    uint64_t rest      = n % 1000;
    int len = 0;

    if (thousands == 1) {                 /* mil */
        len += 3;
    } else {
        len += count_letters_upto_999((int)thousands);
        len += 3;                          /* mil */
    }

    if (rest != 0)
        len += count_letters_upto_999((int)rest);

    return len;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t found = 0;
    uint64_t candidate = 2;                /* first prime to test */

    for (;; ++candidate) {
        if (!is_prime_small((seq_t)candidate))
            continue;

        int lcnt = count_letters(candidate);
        if (is_prime_small((seq_t)lcnt)) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
    }
}
