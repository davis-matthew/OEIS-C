/*
 * A232897  Number of nonnegative integers not exceeding n which have no
 *          decimal digits occurring in n.
 * https://oeis.org/A232897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = |{ m : 0 ≤ m ≤ n, decimal digits(m) ∩ decimal digits(n) = Ø }| */
/* Digit‑DP: for each n we build the forbidden‑digit mask from n’s
   decimal representation and count the numbers not exceeding n that use
   none of those digits. */
#include <string.h>

seq_t a(idx_t n)
{
    /* ---- Collect decimal digits of n (most‑significant first) ---- */
    unsigned char digits[20];          /* n fits in 64 bits → ≤20 digits   */
    int len = 0;
    idx_t tmp = n;

    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = (unsigned char)(tmp % 10);
            tmp /= 10;
        }
        /* reverse to most‑significant first */
        for (int i = 0; i < len / 2; ++i) {
            unsigned char c = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = c;
        }
    }

    /* ---- Build forbidden‑digit bitmask ---- */
    seq_t forbid = 0;
    for (int i = 0; i < len; ++i)
        forbid |= ((seq_t)1 << digits[i]);

    /* ---- Digit DP ---- */
    /* dp[pos][tight][started] : count of ways up to position pos (0‑based)
       with the given tightness and whether a non‑zero digit has been used. */
    seq_t dp[21][2][2] = {0};
    dp[0][1][0] = 1;                /* start before any digits, tight = 1, not started */

    for (int pos = 0; pos < len; ++pos) {
        int limit_digit = digits[pos];
        for (int tight = 0; tight <= 1; ++tight) {
            for (int started = 0; started <= 1; ++started) {
                seq_t cur = dp[pos][tight][started];
                if (cur == 0) continue;

                int limit = tight ? limit_digit : 9;
                for (int d = 0; d <= limit; ++d) {
                    int ntight = (tight && (d == limit)) ? 1 : 0;
                    int nstarted = (started || (d != 0)) ? 1 : 0;

                    if (nstarted) {
                        /* Skip digits that are forbidden by n */
                        if (forbid & (((seq_t)1) << d))
                            continue;
                    }
                    dp[pos + 1][ntight][nstarted] += cur;
                }
            }
        }
    }

    /* Count all positive numbers (started == 1) */
    seq_t result = dp[len][0][1] + dp[len][1][1];

    /* Add number 0 if it does not contain a forbidden digit (i.e. digit 0) */
    if (!(forbid & ((seq_t)1 << 0)))
        ++result;

    return result;
}
