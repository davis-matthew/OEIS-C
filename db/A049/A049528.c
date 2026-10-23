/*
 * A049528  Number of letters in n-th prime (in English).
 * https://oeis.org/A049528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, word
 * Formula:   a(n) = number of letters in the English name of the n‑th prime
 *            (hyphens and spaces excluded). The function uses get_prime() to
 *            obtain the n‑th prime and then counts the letters of its English
 *            name
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"

/* Lengths of unit words 0–19.  The zero entry is unused but kept for
   completeness. */
static const int unit_len[20] = {
    4, /* zero   */
    3, /* one    */
    3, /* two    */
    5, /* three  */
    4, /* four   */
    4, /* five   */
    3, /* six    */
    5, /* seven  */
    5, /* eight  */
    4, /* nine   */
    3, /* ten    */
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

/* Lengths of the tens words 20,30,…,90.  Index 0 and 1 are unused. */
static const int tens_len[10] = {
    0, /* unused */
    0, /* unused */
    6, /* twenty  */
    6, /* thirty  */
    5, /* forty   */
    5, /* fifty   */
    5, /* sixty   */
    7, /* seventy */
    6, /* eighty  */
    6  /* ninety  */
};

/* Recursively compute the number of letters in the English name
   of a positive integer n.  The implementation handles numbers up
   to 999 999 which is more than enough for 10 000th prime (104 729). */
static int len_of_number(uint64_t n)
{
    int len = 0;

    /* Thousands place (if any). */
    if (n >= 1000) {
        uint64_t thousands = n / 1000;
        len += len_of_number(thousands);
        len += 8;                /* "thousand" */
        n %= 1000;
        if (n == 0) return len;
    }

    /* Hundreds place (if any). */
    if (n >= 100) {
        uint64_t hundreds = n / 100;
        len += len_of_number(hundreds);
        len += 7;                /* "hundred" */
        n %= 100;
        if (n == 0) return len;
    }

    /* Tens and units. */
    if (n >= 20) {
        len += tens_len[n / 10];
        n %= 10;
        if (n > 0) len += unit_len[n];
    } else if (n > 0) {
        len += unit_len[n];
    }

    return len;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    uint32_t prime = get_prime(n);
    /* prime is guaranteed to be ≥2 for n ≥ 1. */
    int length = len_of_number((uint64_t)prime);
    return (seq_t)length;
}
