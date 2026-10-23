/*
 * A136709  At step n the sequence lists the number of occurrences of digit (n
 *          mod k), with k>0, in all the numbers from 1 to n. Case k=5.
 * https://oeis.org/A136709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = number of occurrences of digit ((n+1) mod 5) in all *
 *            numbers 1 through (n+1). The sequence is indexed with offset 0:
 *            * a(0) corresponds to n = 1 of the natural definition
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Largest number for which digit counts have been accumulated. */
static idx_t max_computed_n = 0;
/* Cumulative counts of each decimal digit 0–9 up to max_computed_n. */
static seq_t digit_cnt[10] = {0};

/* Add the digits of a single decimal number to the cumulative counts. */
static void add_digits(idx_t x)
{
    while (x > 0) {
        int d = (int)(x % 10);
        digit_cnt[d] += (seq_t)1;
        x /= 10;
    }
}

seq_t a(idx_t n)
{
    /* We need digit counts for numbers 1 .. n+1. */
    idx_t target = n + 1;
    if (target > max_computed_n) {
        for (idx_t i = max_computed_n + 1; i <= target; ++i) {
            add_digits(i);
        }
        max_computed_n = target;
    }
    int d = (int)((n + 1) % 5);   /* digit to count */
    return digit_cnt[d];
}
