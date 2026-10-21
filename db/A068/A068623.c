/*
 * A068623  a(1) = 8; a(n+1) = smallest triangular number > a(n) formed by
 *          adding at least one digit to a(n).
 * https://oeis.org/A068623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A068623
   Formula: a(n) = 18*10^(2*n-6) + 45*10^(n-3) + 28   (n >= 5)
   first four terms are produced by a small search:
   a(1)=8, a(2)=28, a(3)=528, a(4)=18528                      */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow10.h"

/* convert a decimal integer into an array of digits (most significant first) */
static void digits_of(seq_t x, uint8_t *buf, size_t *len)
{
    size_t i = 0;
    do {
        buf[i++] = (uint8_t)(x % 10);
        x /= 10;
    } while (x != 0);
    /* reverse to get most significant first */
    for (size_t j = 0; j < i / 2; ++j) {
        uint8_t tmp = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = tmp;
    }
    *len = i;
}

/* Return true if the decimal representation of a is a subsequence of b */
static bool subseq_digits(seq_t a, seq_t b)
{
    uint8_t da[32], db[32];   /* enough for the small initial values */
    size_t la, lb, i, j;
    digits_of(a, da, &la);
    digits_of(b, db, &lb);
    i = j = 0;
    while (i < la && j < lb) {
        if (da[i] == db[j]) {
            ++i;
        }
        ++j;
    }
    return i == la;
}

/* find the next term given previous value (used for n <= 4) */
static seq_t find_next(seq_t prev)
{
    seq_t k = 1;
    for (;;) {
        seq_t tri = (seq_t)k * ((seq_t)k + 1) / 2;
        if (tri <= prev) {
            ++k;
            continue;
        }
        if (subseq_digits(prev, tri)) {
            return tri;
        }
        ++k;
    }
}

/* cache for the first four terms */
static seq_t small_term(idx_t n)
{
    static seq_t cache[5] = {0, 8, 0, 0, 0};   /* cache[1] = 8 */
    if (cache[n] != (seq_t)0) {
        return cache[n];
    }
    seq_t prev = small_term(n - 1);
    seq_t val = find_next(prev);
    cache[n] = val;
    return val;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;               /* outside of offset */
    }
    if (n <= 4) {
        return small_term(n);
    }

    /* n >= 5 : use the closed‑form formula */
    idx_t exp1 = 2 * n - 6;
    idx_t exp2 = n - 3;
    seq_t part1 = (seq_t)18 * pow10(exp1);
    seq_t part2 = (seq_t)45 * pow10(exp2);
    return part1 + part2 + (seq_t)28;
}
