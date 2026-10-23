/*
 * A126207  Number of 5's in decimal expansion of 5^n.
 * https://oeis.org/A126207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of decimal digit 5's in 5^n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state: current maximum computed n, digit array and counts array */
    static idx_t computed_max = 0;          /* last n for which counts are known */
    static seq_t *counts = NULL;            /* array of a(k) for k <= computed_max */
    static size_t counts_cap = 0;           /* capacity of counts array */

    static unsigned char *digits = NULL;    /* decimal digits of 5^current_i, low to high */
    static size_t digits_len = 0;           /* number of used digits */
    static size_t digits_cap = 0;           /* capacity of digits array */

    /* Initialisation on first call */
    if (counts == NULL) {
        /* allocate for n=0 */
        counts_cap = 1;
        counts = (seq_t*)malloc(counts_cap * sizeof(seq_t));
        if (!counts) return (seq_t)0; /* allocation failure handled conservatively */
        counts[0] = (seq_t)0;                 /* a(0) = 0 */
        computed_max = 0;

        /* initialise digit array for 5^0 = 1 */
        digits_cap = 16;
        digits = (unsigned char*)malloc(digits_cap * sizeof(unsigned char));
        if (!digits) return (seq_t)0;
        digits[0] = 1;
        digits_len = 1;
    }

    /* If requested n already known, return it immediately */
    if (n <= computed_max)
        return counts[n];

    /* Ensure counts array can hold up to n */
    if ((size_t)n + 1 > counts_cap) {
        size_t new_cap = counts_cap == 0 ? 1 : counts_cap;
        while ((size_t)n + 1 > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(counts, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        counts = tmp;
        counts_cap = new_cap;
    }

    /* Multiply current 5^k by 5 to get 5^(k+1) iteratively */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        /* Ensure digits buffer has space for potential new digit */
        if (digits_len + 1 > digits_cap) {
            size_t new_cap = digits_cap == 0 ? 16 : digits_cap;
            while (digits_len + 1 > new_cap)
                new_cap *= 2;
            unsigned char *tmp = (unsigned char*)realloc(digits, new_cap * sizeof(unsigned char));
            if (!tmp) return (seq_t)0; /* allocation failure */
            digits = tmp;
            digits_cap = new_cap;
        }

        /* Multiply decimal digits by 5 */
        unsigned int carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            unsigned int prod = (unsigned int)digits[i] * 5 + carry;
            digits[i] = (unsigned char)(prod % 10);
            carry     = prod / 10;
        }
        if (carry) {
            digits[digits_len++] = (unsigned char)carry;
        }

        /* Count how many digits are 5 */
        seq_t cnt = 0;
        for (size_t i = 0; i < digits_len; ++i)
            if (digits[i] == 5)
                ++cnt;

        /* Store the result for this n */
        counts[k] = cnt;
    }

    computed_max = n;
    return counts[n];
}
