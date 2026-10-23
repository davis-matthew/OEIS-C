/*
 * A137180  Number of palindromes in the range [1,n] inclusive.
 * https://oeis.org/A137180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of decimal palindromes in the range [1,n]
 *            inclusive
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   rev_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: count palindromes by digit length. For the final length,
 * construct the largest palindrome ≤ n from its first half and decide whether
 * to include that half.  Complexity O(number of digits of n). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/rev_seq.h"

/* ---------- small helpers -------------------------------------------- */

static inline seq_t pow10_seq(unsigned e)
{
    seq_t p = 1;
    while (e--) p *= 10;
    return p;
}

/* count decimal digits of x (x ≥ 0); returns at least 1 */
static inline unsigned digit_count_seq(seq_t x)
{
    unsigned c = 0;
    do {
        c++;
        x /= 10;
    } while (x > 0);
    return c;
}

/* number of palindromes with exactly L decimal digits (L ≥ 1) */
static inline seq_t count_len(unsigned L)
{
    if (L == 1) return (seq_t)9;
    unsigned half = (L + 1) >> 1;          /* ceil(L/2) */
    return (seq_t)9 * pow10_seq(half - 1);
}

/* ---------- main sequence function ----------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* offset 0 */

    seq_t n_seq = (seq_t)n;

    /* total digits of n */
    unsigned digits = digit_count_seq(n_seq);

    seq_t sum = 0;

    /* add all palindromes with fewer digits */
    for (unsigned L = 1; L < digits; ++L)
        sum += count_len(L);

    /* handle palindromes with exactly `digits` digits */
    unsigned half = (digits + 1) >> 1;          /* ceil(digits/2) */
    seq_t base   = pow10_seq(digits - half);    /* 10^(digits-half) */
    seq_t first_half = n_seq / base;            /* most significant half */
    seq_t start_half = pow10_seq(half - 1);     /* smallest first half */

    seq_t palindrome;
    if ((digits & 1) == 0) {                   /* even length */
        palindrome = first_half * base + rev_seq(first_half);
    } else {                                   /* odd length */
        palindrome = first_half * base + rev_seq(first_half / 10);
    }

    seq_t same_len_count;
    if (palindrome <= n_seq)
        same_len_count = first_half - start_half + 1;
    else
        same_len_count = first_half - start_half;

    sum += same_len_count;
    return sum;
}
