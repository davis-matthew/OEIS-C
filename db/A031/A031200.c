/*
 * A031200  Periods of sum of 7th powers of digits iterated until the sequence
 *          becomes periodic.
 * https://oeis.org/A031200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the length of the eventual cycle obtained by repeatedly
 * replacing x by the sum of the seventh powers of its decimal digits.
 * a(n) = period of the sequence starting from n.          */

#include <stdint.h>
#include <stddef.h>

static const seq_t pow7[10] = {
    (seq_t)0ULL,               /* 0^7   */
    (seq_t)1ULL,               /* 1^7   */
    (seq_t)128ULL,             /* 2^7   */
    (seq_t)2187ULL,            /* 3^7   */
    (seq_t)16384ULL,           /* 4^7   */
    (seq_t)78125ULL,           /* 5^7   */
    (seq_t)279936ULL,          /* 6^7   */
    (seq_t)823543ULL,          /* 7^7   */
    (seq_t)2097152ULL,         /* 8^7   */
    (seq_t)4782969ULL          /* 9^7   */
};

static inline seq_t digit_sum7(seq_t x)
{
    seq_t sum = 0;
    /* Special case: when x == 0 the sum of digits^7 is 0          */
    while (x > 0) {
        /* x % 10 is a seq_t value not exceeding 9; cast to int   */
        int d = (int)(x % 10);
        sum += pow7[d];
        x /= 10;
    }
    return sum;
}

static inline seq_t f(seq_t x)
{
    return digit_sum7(x);
}

/* Public API as required. */
seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;

    /* Tortoise–hare algorithm to find a meeting point inside the cycle. */
    seq_t tortoise = f(start);
    seq_t hare = f(f(start));

    while (tortoise != hare) {
        tortoise = f(tortoise);
        hare = f(f(hare));
    }

    /* Compute the period λ. */
    seq_t lam = 1;
    hare = f(tortoise);
    while (tortoise != hare) {
        hare = f(hare);
        lam++;
    }
    return lam;
}
