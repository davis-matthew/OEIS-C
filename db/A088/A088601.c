/*
 * A088601  Number of steps to reach 0 when iterating A261424(x) = x - (the
 *          largest palindrome less than x), starting at n.
 * https://oeis.org/A088601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the number of iterations of * f(x) = x - L(x), where
 *            L(x) is the largest palindrome ≤ x, * until the value reaches 0,
 *            starting from n
 * Range:     u8 n <= 3300, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10, reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"   /* seq_t pow10(idx_t) */
#include "../utils/reverse.h" /* seq_t reverse(seq_t v) */

/* Find the number of decimal digits of a non‑negative integer. */
static inline idx_t digit_len(seq_t x) {
    idx_t len = 0;
    while (x > 0) {
        x = x / 10;
        len++;
    }
    return len > 0 ? len : 1;   /* x==0 never occurs for this sequence */
}

/* Largest palindrome ≤ x (x ≥ 1). */
static seq_t largest_palindrome_leq(seq_t x) {
    if (x < 10) return x;          /* single digit numbers are palindromes */

    idx_t d = digit_len(x);         /* number of digits of x */
    idx_t l = (d + 1) / 2;          /* ceil(d/2) */

    seq_t pow_d_l = pow10(d - l);   /* 10^(d-l) */
    seq_t left = x / pow_d_l;       /* most significant l digits */

    /* Build candidate palindrome from 'left'. */
    seq_t cand;
    if (d % 2 == 0) {              /* even number of digits */
        seq_t rev = reverse(left);
        cand = left * pow10(l) + rev;
    } else {                       /* odd number of digits */
        seq_t rev = reverse(left / 10);
        cand = left * pow10(l - 1) + rev;
    }

    if (cand <= x) return cand;

    /* Decrement the left part and rebuild. */
    left -= (seq_t)1;
    if (left < pow10(l - 1)) {     /* length shrank -> leading zero */
        /* Largest palindrome < 10^d is all 9's of length d-1. */
        return pow10(d - 1) - (seq_t)1;
    }

    if (d % 2 == 0) {
        seq_t rev = reverse(left);
        cand = left * pow10(l) + rev;
    } else {
        seq_t rev = reverse(left / 10);
        cand = left * pow10(l - 1) + rev;
    }
    return cand;
}

/**
 * External function required by the test harness.
 * Computes a(n) for n >= 1.
 */
seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t steps = (seq_t)0;
    while (x > (seq_t)0) {
        seq_t pal = largest_palindrome_leq(x);
        x -= pal;
        steps++;
    }
    return steps;
}
