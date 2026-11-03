/*
 * A065571  Numbers whose decimal digits can be permuted to give a multiple of
 *          11.
 * https://oeis.org/A065571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 53 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Check whether the decimal digits of a number can be permuted to form a multiple of 11 */
/* a(0) = 0, a(n) is the nth smallest non‑negative integer with that property. */
/* The algorithm tests each integer in turn (starting at 0) and uses a DP over
   digit subsets to decide if a permutation divisible by 11 exists. */

#include <stdbool.h>
#include <stdint.h>

/* Helper: true if some permutation of the decimal digits of x is divisible by 11. */
static bool has_11_permutation(uint64_t x)
{
    /* Extract digits into an array. */
    int digits[20];
    int len = 0;
    if (x == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (x > 0) {
            digits[len++] = x % 10;
            x /= 10;
        }
    }

    /* Sum of digits modulo 11. */
    int sum_mod = 0;
    for (int i = 0; i < len; ++i)
        sum_mod = (sum_mod + digits[i]) % 11;

    /* We need a subset of k = ceil(len/2) digits whose sum ≡ T (mod 11),
       where 2*sum_odd ≡ sum_digits (mod 11). 2⁻¹ mod 11 = 6. */
    int k = (len + 1) / 2;
    int target = (sum_mod * 6) % 11;

    /* DP[mod][cnt] : can we reach 'mod' with exactly 'cnt' chosen digits? */
    bool dp[11][11] = {{0}};
    dp[0][0] = true;

    for (int idx = 0; idx < len; ++idx) {
        int d = digits[idx];
        bool ndp[11][11] = {{0}};
        for (int m = 0; m < 11; ++m) {
            for (int c = 0; c <= k; ++c) {
                if (!dp[m][c]) continue;
                /* Exclude this digit (still possible). */
                ndp[m][c] = true;
                /* Include this digit. */
                int nm = (m + d) % 11;
                if (c + 1 <= k)
                    ndp[nm][c + 1] = true;
            }
        }
        /* copy back */
        for (int m = 0; m < 11; ++m)
            for (int c = 0; c <= k; ++c)
                dp[m][c] = ndp[m][c];
    }
    return dp[target][k];
}

/* OEIS sequence A065571 */
seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t num = 0;
    while (found <= n) {
        if (has_11_permutation(num)) {
            if (found == n)
                return (seq_t)num;
            ++found;
        }
        ++num;
    }
    /* should never reach here */
    return (seq_t)0;
}
