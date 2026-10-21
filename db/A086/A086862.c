/*
 * A086862  Differences between successive palindromes.
 * https://oeis.org/A086862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A002113(n+1) – A002113(n) where A002113 is the sequence
 *            of decimal palindromes, starting with 0
 * Range:     u8 n <= 1098, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10_idx.h"

static inline seq_t pow10_seq_t(int exp) {
    seq_t v = 1;
    while (exp--) v *= 10;
    return v;
}

/* Compute the n-th decimal palindrome (A002113(n)), with A002113(1)=0. */
static seq_t palindrome(idx_t n) {
    if (n <= 10) {
        /* Palindromes 0 … 9 */
        return (seq_t)(n - 1);
    }

    idx_t rem = n - 10;                 /* remaining index after the first 10 */
    int L = 2;                          /* current palindrome length */

    while (1) {
        int halfExp = (L % 2 == 0) ? (L / 2 - 1) : ((L - 1) / 2);
        idx_t count = 9 * pow10_idx(halfExp);  /* palindromes of length L */

        if (rem <= count)
            break;

        rem -= count;
        ++L;
    }

    int halfLen = (L + 1) / 2;          /* digits in the left (and middle) part */
    seq_t leftBase = pow10_seq_t(halfLen - 1);
    seq_t left = leftBase + (seq_t)(rem - 1);

    seq_t rev = 0;
    seq_t tmp = left;

    if (L % 2 == 0) {                  /* even length: mirror all of left */
        while (tmp) {
            rev = rev * 10 + (tmp % 10);
            tmp /= 10;
        }
        return left * pow10_seq_t(halfLen) + rev;
    } else {                            /* odd length: skip middle digit */
        tmp /= 10;                      /* drop the middle digit */
        while (tmp) {
            rev = rev * 10 + (tmp % 10);
            tmp /= 10;
        }
        return left * pow10_seq_t(halfLen - 1) + rev;
    }
}

/* External function required by the framework. */
seq_t a(idx_t n) {
    /* a(n) = palindrome(n+1) – palindrome(n) */
    return palindrome(n + 1) - palindrome(n);
}
