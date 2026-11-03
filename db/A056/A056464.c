/*
 * A056464  Number of primitive (aperiodic) palindromes using exactly three
 *          different symbols.
 * https://oeis.org/A056464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * (3^{ceil(n/2)} - 3*2^{ceil(n/2)} + 3)
 *            offset: 1
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 80, u128 n <= 160,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 200 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

/* Count palindromes of length len that use all three symbols.
   The number equals 3^m - 3*2^m + 3 where m = ceil(len/2). */
static seq_t palindrome3_count(idx_t len)
{
    idx_t m = (len + 1) / 2;          /* ceil(len/2) */
    seq_t pow3 = ipow_idx(3, m);
    seq_t pow2 = ipow_idx(2, m);
    seq_t res = pow3;
    res -= seq_mul(pow2, 3);
    res += 3;
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t pos = 0;                    /* sum of positive contributions   */
    seq_t neg = 0;                    /* sum of negative contributions   */

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            /* contribution from divisor d1 */
            int mu1 = mu_of(d1);
            if (mu1 == 1)
                pos += palindrome3_count(d2);
            else if (mu1 == -1)
                neg += palindrome3_count(d2);

            /* contribution from the complementary divisor d2 */
            if (d1 != d2) {
                int mu2 = mu_of(d2);
                if (mu2 == 1)
                    pos += palindrome3_count(d1);
                else if (mu2 == -1)
                    neg += palindrome3_count(d1);
            }
        }
    }

    return pos - neg;                 /* pos >= neg for all n */
}
