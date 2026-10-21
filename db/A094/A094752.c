/*
 * A094752  Largest k such that 2n/k + 1 as well as 2nk+1 are prime, or 0 if
 *          no such number exists.
 * https://oeis.org/A094752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest divisor k of 2n such that * 2n/k + 1 and 2n*k + 1
 *            are both prime; 0 if no such k exists
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t two_n = 2 * n;          /* 2n */
    seq_t best = 0;               /* best k found */

    for (idx_t d = 1; d * d <= two_n; ++d) {
        if (two_n % d != 0)
            continue;                       /* d is not a divisor of 2n */

        /* divisor d */
        idx_t k = d;
        idx_t a_val = two_n / k + 1;
        /* 2n*k fits in 64‑bit for all n considered (<=10 000) */
        idx_t b_val = two_n * k + 1;
        if (is_prime(a_val) && is_prime(b_val) && k > best)
            best = (seq_t)k;

        /* paired divisor two_n / d (if distinct) */
        idx_t d2 = two_n / d;
        if (d2 != d) {
            k = d2;
            a_val = two_n / k + 1;
            b_val = two_n * k + 1;
            if (is_prime(a_val) && is_prime(b_val) && k > best)
                best = (seq_t)k;
        }
    }
    return best;                      /* 0 if no suitable divisor was found */
}
