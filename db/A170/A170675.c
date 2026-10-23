/*
 * A170675  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 42 * 41^(n-1) for 1 ≤ n < 49 a(n) = 40 *
 *            Σ_{k=1}^{48} a(n-k) - 820 * a(n-49) for n ≥ 49 (This follows
 *            from the rational generating function of A170675.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n < 49) {
        seq_t pow = big_pow((seq_t)41, (seq_t)(n - 1));
        return ((seq_t)42) * pow;
    }

    /* pre‑compute the first 49 terms (0 … 48) */
    seq_t buf[49];
    buf[0] = (seq_t)1;
    for (idx_t i = 1; i < 49; ++i) {
        buf[i] = ((seq_t)42) * big_pow((seq_t)41, (seq_t)(i - 1));
    }

    /* sum of the last 48 terms a(1)…a(48) */
    seq_t windowSum = (seq_t)0;
    for (idx_t i = 1; i < 49; ++i)
        windowSum += buf[i];

    seq_t result = (seq_t)0;
    for (idx_t i = 49; i <= n; ++i) {
        /* a(i) = 40*sum_{k=1}^{48} a(i-k) - 820*a(i-49) */
        seq_t term = ((seq_t)40) * windowSum - ((seq_t)820) * buf[i - 49];
        buf[i % 49] = term;

        /* update the sliding window of 48 terms */
        seq_t oldOut = buf[(i - 48) % 49]; /* a(i-48) that leaves the window */
        windowSum += term;
        windowSum -= oldOut;

        result = term;
    }
    return result;
}
