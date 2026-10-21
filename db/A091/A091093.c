/*
 * A091093  In ternary representation: minimal number of editing steps
 *          (delete, insert or substitute) to transform n into n^2.
 * https://oeis.org/A091093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = LevenshteinDistance(ternary(n), ternary(n^2))
 * Range:     u8 n <= 15, u16 n <= 475, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define MAX_DIGITS 1024    /* enough for any seq_t that fits in memory */

/* Convert a number to base‑3 digits, most significant first.
   Digits are stored in the array 'digits', length returned in '*len'.
   The array must be at least MAX_DIGITS in size. */
static void get_digits(seq_t value, uint8_t *digits, size_t *len)
{
    if (value == 0) {
        digits[0] = 0;
        *len = 1;
        return;
    }
    size_t i = 0;
    while (value > 0 && i < MAX_DIGITS) {
        digits[i++] = (uint8_t)(value % 3);
        value /= 3;
    }
    /* reverse to most–significant first */
    for (size_t j = 0; j < i / 2; ++j) {
        uint8_t tmp = digits[j];
        digits[j] = digits[i - 1 - j];
        digits[i - 1 - j] = tmp;
    }
    *len = i;
}

/* Levenshtein distance between two sequences of digits (0..2). */
static int levenshtein(const uint8_t *a, size_t na, const uint8_t *b, size_t nb)
{
    int prev[MAX_DIGITS + 1];
    int cur [MAX_DIGITS + 1];

    for (int j = 0; j <= (int)nb; ++j) prev[j] = j;

    for (size_t i = 1; i <= na; ++i) {
        cur[0] = (int)i;
        for (size_t j = 1; j <= nb; ++j) {
            int cost = (a[i-1] == b[j-1]) ? 0 : 1;
            int del  = prev[j] + 1;
            int ins  = cur[j-1] + 1;
            int sub  = prev[j-1] + cost;
            int m = del;
            if (ins < m) m = ins;
            if (sub < m) m = sub;
            cur[j] = m;
        }
        /* swap prev and cur */
        int *tmp = prev;
        for (int k = 0; k <= (int)nb; ++k) tmp[k] = cur[k];
    }
    return prev[nb];
}

seq_t a(idx_t n)
{
    /* n and n^2 in seq_t */
    seq_t nsq = ((seq_t)n) * ((seq_t)n);

    uint8_t d1[MAX_DIGITS];
    uint8_t d2[MAX_DIGITS];
    size_t l1, l2;

    get_digits((seq_t)n, d1, &l1);
    get_digits(nsq, d2, &l2);

    int dist = levenshtein(d1, l1, d2, l2);
    return (seq_t)dist;
}
