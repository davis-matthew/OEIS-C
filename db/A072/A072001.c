/*
 * A072001  Variant of the factorial base representation of n.
 * https://oeis.org/A072001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Modeled after OEIS A072001:  a(n) is the concatenation of the
 * factorial‑base digits of (n‑1) (starting with 1! and higher), followed
 * by a final digit 1.  For example
 *  a(3) = 101  (digits of 2 are 10, append 1)
 *  a(7) = 1001 (digits of 6 are 100, append 1) */

#include <stdint.h>
#include "../utils/digit_count.h"
#include "../utils/pow10.h"

/* Compute A072001 for index n (n ≥ 1).  For n = 1 the result is 1. */
seq_t a(idx_t n)
{
    /* Special case for the first value; the algorithm below also
     * works for n = 1, but the explicit check keeps the intent
     * clear. */
    if (n == 1)
        return (seq_t)1;

    /* n-1 is the number we convert to factorial base (digits d_k
     * for k = 1 .. m, with 0 ≤ d_k ≤ k). */
    idx_t n1 = n - 1;

    /* Compute factorials up to the highest k such that k! ≤ n1.
     * 20! fits in a 64‑bit unsigned type; for all n ≤ 2^64‑1
     * this bound is sufficient. */
    idx_t fact[21];
    fact[0] = (idx_t)1;          /* 0! = 1, unused but convenient   */
    int m = 0;                   /* largest k with k! ≤ n1 (0 if n1 = 0) */

    for (int i = 1; i <= 20; ++i) {
        /* Avoid overflow: only compute if fact[i‑1] * i does not
         * exceed n1.  Since n1 is an idx_t, this guard keeps
         * multiplication in bounds. */
        if (fact[i-1] > n1 / (idx_t)i)
            break;
        fact[i] = fact[i-1] * (idx_t)i;
        if (fact[i] <= n1)
            m = i;
    }

    /* Now obtain the factorial‑base digits d_k for k = m … 1.
     * They satisfy 0 ≤ d_k ≤ k, and n1 = Σ d_k k!. */
    uint8_t digits[21] = {0};
    for (int k = m; k >= 1; --k) {
        facts:   ;
        idx_t f   = fact[k];
        idx_t d   = n1 / f;
        n1        = n1 % f;
        digits[k] = (uint8_t)d;
    }

    /* Assemble the decimal value by concatenating each d_k as a
     * decimal string, then appending the final digit 1 (d_0). */
    seq_t res = (seq_t)0;

    for (int k = m; k >= 1; --k) {
        uint8_t d = digits[k];
        /* Compute the number of decimal digits of d.
         * 0 must be counted as a single digit. */
        int dlen = digit_count((uint64_t)d);
        if (d == 0)
            dlen = 1;
        seq_t mult = pow10((idx_t)dlen);   /* 10^dlen as seq_t */
        res = res * mult + (seq_t)d;       /* shift left by dlen places and add d */
    }

    /* Append the mandatory low‑order digit 1. */
    res = res * (seq_t)10 + (seq_t)1;

    return res;
}
