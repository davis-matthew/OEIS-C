/*
 * A087036  Lunar cubes: n*n*n, where * is lunar multiplication.
 * https://oeis.org/A087036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * n in lunar arithmetic, where the lunar product of two
 *            numbers is defined as follows: For the decimal digits of numbers
 *            A = a₀a₁…a_{k-1} and B = b₀b₁…b_{k-1} (most‑significant digit
 *            first), the product A * B consists of the digits min(aᵢ, bⱼ)
 *            produced in the order: for each digit bⱼ of...
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* First, extract the decimal digits of n, most‑significant first. */
    int digits[20];          /* enough for uint64_t */
    int dlen = 0;

    if (n == 0) {
        digits[0] = 0;
        dlen = 1;
    } else {
        idx_t tmp = n;
        /* Collect digits in reverse order (least‑significant first). */
        while (tmp > 0) {
            digits[dlen++] = tmp % 10;
            tmp /= 10;
        }
        /* Reverse to most‑significant order. */
        for (int i = 0; i < dlen / 2; ++i) {
            int t = digits[i];
            digits[i] = digits[dlen - 1 - i];
            digits[dlen - 1 - i] = t;
        }
    }

    /* Compute the lunar square: digits of the result. */
    int res_digits[125];      /* upper bound: 5*5 = 25, but leave margin */
    int rlen = 0;
    for (int j = 0; j < dlen; ++j) {          /* outer loop over B's digits */
        int bj = digits[j];
        for (int i = 0; i < dlen; ++i) {      /* inner loop over A's digits */
            int ai = digits[i];
            int m = ai < bj ? ai : bj;        /* min(ai, bj) */
            res_digits[rlen++] = m;
        }
    }

    /* Convert the digit array back to a seq_t value. */
    seq_t result = 0;
    for (int i = 0; i < rlen; ++i) {
        result = result * 10 + res_digits[i];
    }

    return result;
}
