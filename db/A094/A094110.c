/*
 * A094110  Start with the word "One". The next word is the number of letters
 *          written previously. Convert this infinite sequence of words into
 *          an infinite sequence of numbers.
 * https://oeis.org/A094110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(1) = 1, a(n) = a(n−1) + letter_count(a(n−1)) for n ≥ 2, where
 *            letter_count(m) is the number of letters in the English word for
 *            the non‑negative integer m (ignoring hyphens and spaces)
 * Range:     u8 n <= 22, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- Letter counting utility ------------------------------------------ */
#include <stdlib.h>
#include <stdint.h>

/* Letter counts for 0–19 (index 0 is unused). */
static const unsigned char ones[20] = {
    0, 3, 3, 5, 4, 4, 3, 5, 5, 4,
    3, 6, 6, 8, 8, 7, 7, 9, 8, 8
};

/* Letter counts for the tens words 20,30,…,90 (index 2–9).  */
static const unsigned char tens_words[10] = {
    0, 0, 6, 6, 5, 5, 5, 7, 6, 6
};

/* Return the number of letters in the English word for n (n ≥ 0).  */
static seq_t letter_count(idx_t n)
{
    if (n == 0) return 0;
    if (n < 20) return ones[(int)n];

    if (n < 100) {                         /* 20…99 */
        int tens = n / 10;
        int rem  = n % 10;
        return tens_words[tens] + (rem ? ones[rem] : 0);
    }

    if (n < 1000) {                        /* 100…999 */
        int hundreds = n / 100;
        int rem = n % 100;
        seq_t count = ones[hundreds] + 7;   /* "hundred" = 7 letters */
        if (rem) count += letter_count(rem);
        return count;
    }

    /* thousands and up (handles any size that fits in idx_t) */
    int thousands = n / 1000;
    int rem = n % 1000;
    seq_t count = letter_count(thousands) + 8;  /* "thousand" = 8 letters */
    if (rem) count += letter_count(rem);
    return count;
}

/* --------------------------------------------------------------------- */
/* Sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                          /* offset 1 – unused data */

    static idx_t max_n = 0;
    static seq_t *seq_arr = NULL;                  /* a[1..] */
    static seq_t *cum_arr = NULL;                  /* cumulative sums */

    if (n > max_n) {
        size_t new_size = (size_t)n + 1;           /* allocate up to n */
        seq_arr = (seq_t *)realloc(seq_arr,
                                   new_size * sizeof(seq_t));
        cum_arr = (seq_t *)realloc(cum_arr,
                                   new_size * sizeof(seq_t));

        if (max_n == 0) {                         /* initialise */
            seq_arr[1] = (seq_t)1;
            cum_arr[1] = letter_count((idx_t)1);
            max_n = 1;
        }

        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t prev_a = seq_arr[i - 1];        /* a(i-1) */
            seq_t curr_a = cum_arr[i - 1];        /* a(i)  = sum_{k< i} lc(a(k)) */
            seq_arr[i] = curr_a;
            seq_t letters = letter_count((idx_t)curr_a);
            cum_arr[i] = cum_arr[i - 1] + letters;
        }
        max_n = n;
    }

    return seq_arr[n];
}
