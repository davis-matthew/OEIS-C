/*
 * A073327  Write U.S. English name for n (ignoring hyphens and spaces) and
 *          add numerical values of letters using a=1, b=2, ..., y=25, z=26.
 * https://oeis.org/A073327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, look
 * Formula:   a(n) is computed by writing the U.S. English name of n *
 *            (without “and”) and adding the values of its letters * (a=1,
 *            b=2, …, z=26). Hyphens and spaces are ignored
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   word_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include "../utils/word_sum.h"

/* Names for 0–19 */
static const char *const small[] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};

/* Tens names 20,30,...,90 */
static const char *const tens[] = {
    "", "", "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
};

/* Recursive helper to compute sum of letters for any non‑negative integer */
static seq_t sum_name(idx_t n)
{
    if (n < 20)
        return word_sum(small[n]);

    if (n < 100) {
        idx_t t = n / 10;
        idx_t r = n % 10;
        seq_t sum = word_sum(tens[t]);
        if (r)
            sum += word_sum(small[r]);
        return sum;
    }

    if (n < 1000) {
        idx_t h = n / 100;
        idx_t r = n % 100;
        seq_t sum = word_sum(small[h]) + word_sum("hundred");
        if (r)
            sum += sum_name(r);
        return sum;
    }

    if (n < 1000000) { /* < 1,000,000 */
        idx_t th = n / 1000;
        idx_t r = n % 1000;
        seq_t sum = sum_name(th) + word_sum("thousand");
        if (r)
            sum += sum_name(r);
        return sum;
    }

    if (n < 1000000000) { /* < 1,000,000,000 */
        idx_t mil = n / 1000000;
        idx_t r = n % 1000000;
        seq_t sum = sum_name(mil) + word_sum("million");
        if (r)
            sum += sum_name(r);
        return sum;
    }

    if (n < 1000000000000ULL) { /* < 1,000,000,000,000 */
        idx_t bil = n / 1000000000ULL;
        idx_t r = n % 1000000000ULL;
        seq_t sum = sum_name(bil) + word_sum("billion");
        if (r)
            sum += sum_name(r);
        return sum;
    }

    /* For very large numbers, continue with groups of three digits */
    idx_t grp = n / 1000000000000ULL; /* trillion */
    idx_t r = n % 1000000000000ULL;
    seq_t sum = sum_name(grp) + word_sum("trillion");
    if (r)
        sum += sum_name(r);
    return sum;
}

seq_t a(idx_t n)
{
    /* a(0) = 64 (zero) */
    if (n == 0)
        return 64;
    return sum_name(n);
}
