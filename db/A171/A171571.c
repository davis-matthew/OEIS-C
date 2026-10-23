/*
 * A171571  A050278/9, where A050278 are the pandigital numbers.
 * https://oeis.org/A171571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, nonn
 * Formula:   a(n) = pandigital_n / 9 where pandigital_n is the nth 10‑digit
 *            pandigital number (digits 0–9) with no leading zero. The
 *            sequence has 9*9! = 3265920 terms
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const idx_t fact[11] = {
    1ULL,          /* 0! */
    1ULL,          /* 1! */
    2ULL,          /* 2! */
    6ULL,          /* 3! */
    24ULL,         /* 4! */
    120ULL,        /* 5! */
    720ULL,        /* 6! */
    5040ULL,       /* 7! */
    40320ULL,      /* 8! */
    362880ULL,     /* 9! */
    3628800ULL     /* 10! */
};

seq_t a(idx_t n)
{
    /* Sequence starts at n = 1.  For out of range return 0. */
    if (n == 0) return 0;
    if (n > (idx_t)9 * fact[9]) return 0;

    idx_t k = n - 1;                     /* 0‑based index */
    idx_t first_idx = k / fact[9];       /* 0..8 → first digit 1..9 */
    uint8_t first_digit = (uint8_t)(first_idx + 1);

    idx_t rest_idx = k % fact[9];        /* index among permutations of remaining 9 digits */

    /* Build the 9 remaining digits in sorted order (0,1,...,9) without the first digit */
    uint8_t avail[9];
    idx_t pos = 0;
    for (uint8_t d = 0; d <= 9; ++d) {
        if (d == first_digit) continue;
        avail[pos++] = d;
    }

    /* Generate the permutation of the remaining digits using Lehmer code */
    uint8_t perm[9];
    uint8_t tmp[9];
    for (int i = 0; i < 9; ++i) tmp[i] = avail[i];

    idx_t t = rest_idx;
    for (int i = 9; i >= 1; --i) {
        idx_t f = fact[i - 1];
        idx_t digit_index = t / f;
        t %= f;
        perm[9 - i] = tmp[digit_index];

        /* Remove selected digit from tmp */
        for (int j = digit_index; j < i - 1; ++j) {
            tmp[j] = tmp[j + 1];
        }
    }

    /* Construct the full pandigital number and divide by 9 */
    seq_t num = first_digit;
    for (int i = 0; i < 9; ++i) {
        num = num * 10 + perm[i];
    }
    return num / (seq_t)9;
}
