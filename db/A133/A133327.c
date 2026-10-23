/*
 * A133327  Indices of the 7-gonal numbers that are the sum of 2 consecutive
 *          7-gonal numbers.
 * https://oeis.org/A133327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: for the odd subsequence (indices n = 1,3,5,...) let
   b(1)=1   (n=1),  b(2)=19265 (n=3);
   for k >= 3:  b(k) = 39202 * b(k-1) - b(k-2) - 11760.
   For the even subsequence (indices n = 2,4,6,...) let
   c(1)=241   (n=2),  c(2)=9435905 (n=4);
   for k >= 3:  c(k) = 39202 * c(k-1) - c(k-2) - 11760.
   The function returns a(n) according to this rule. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)241;

    /* Constants for the recurrence, cast to seq_t to preserve type. */
    const seq_t mult   = (seq_t)39202;
    const seq_t sub    = (seq_t)11760;

    if (n & 1) { /* odd index: use the odd subsequence */
        /* The k-th odd index corresponds to n = 2*k-1. */
        idx_t k = (n + 1) / 2;  /* 1-based position in the odd subsequence */
        seq_t prev2 = (seq_t)1;            /* b(1) */
        seq_t prev1 = (seq_t)19265;        /* b(2) */

        if (k == 1) return prev2;
        if (k == 2) return prev1;

        for (idx_t i = 3; i <= k; ++i) {
            seq_t cur = prev1 * mult;
            cur -= prev2;
            cur -= sub;
            prev2 = prev1;
            prev1 = cur;
        }
        return prev1;
    } else { /* even index: use the even subsequence */
        /* The k-th even index corresponds to n = 2*k. */
        idx_t k = n / 2;  /* 1-based position in the even subsequence */
        seq_t prev2 = (seq_t)241;          /* c(1) */
        seq_t prev1 = (seq_t)9435905;      /* c(2) */

        if (k == 1) return prev2;
        if (k == 2) return prev1;

        for (idx_t i = 3; i <= k; ++i) {
            seq_t cur = prev1 * mult;
            cur -= prev2;
            cur -= sub;
            prev2 = prev1;
            prev1 = cur;
        }
        return prev1;
    }
}
