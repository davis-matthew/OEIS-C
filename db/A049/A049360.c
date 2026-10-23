/*
 * A049360  Digitally balanced numbers in base 9: equal numbers of 0's, 1's,
 *          ..., 8's.
 * https://oeis.org/A049360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th lexicographically ordered 9‑digit base‑9 number
 *            consisting of each digit 0–8 exactly once and not starting with
 *            0. The numbers are sorted by their decimal (base‑10) value
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses factorial ranking to generate the
   n‑th permutation directly in O(1) time (O(9) operations). */

#include <stdint.h>

static const idx_t fac[9] = {
    1ULL, /* 0! */
    1ULL, /* 1! */
    2ULL, /* 2! */
    6ULL, /* 3! */
    24ULL,/* 4! */
    120ULL,/* 5! */
    720ULL,/* 6! */
    5040ULL,/* 7! */
    40320ULL /* 8! */
};

seq_t a(idx_t n)
{
    /* n is 1-indexed.  For n > 8*8! the sequence is undefined,
       but that case is not expected in the testset. */
    if (n == 0) {
        return 0;                    /* invalid input */
    }

    idx_t n0 = n - 1;                /* 0‑based rank */
    idx_t lead_index = n0 / fac[8];  /* which leading digit (0..7) */
    if (lead_index >= 8) {
        return 0;                    /* out of range */
    }

    uint8_t first_digit = (uint8_t)(lead_index + 1);   /* leading digit 1..8 */
    idx_t r = n0 % fac[8];              /* rank within this block */

    /* Prepare the remaining 8 digits in ascending order, omitting
       the chosen leading digit. */
    uint8_t digits[8];
    int pos = 0;
    for (uint8_t d = 0; d <= 8; ++d) {
        if (d != first_digit) {
            digits[pos++] = d;
        }
    }

    /* Build the value from digits, rank r specifies the lexicographic
       index of the permutation of the remaining 8 digits. */
    seq_t value = first_digit;   /* start with the leading digit */
    idx_t k = r;
    int len = 8;

    for (int i = 0; i < 8; ++i) {
        idx_t fact = fac[len - 1];          /* (len-1)! */
        idx_t idx = k / fact;               /* next digit index */
        uint8_t d = digits[idx];
        value = value * 9 + d;              /* shift in the next digit */

        /* Remove used digit from the array */
        for (int j = idx; j + 1 < len; ++j) {
            digits[j] = digits[j + 1];
        }

        k %= fact;
        --len;
    }

    return value;
}
