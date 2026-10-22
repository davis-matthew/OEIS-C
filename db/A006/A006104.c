/*
 * A006104  Gaussian binomial coefficient [ n,n/2 ] for q=3.
 * https://oeis.org/A006104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n, floor(n/2)]_3 = Product_{i=1..k} (3^(n-k+i) - 1)/(3^i
 *            - 1), k = floor(n/2); computed incrementally as [n-k+i, i]_3 =
 *            [n-k+i-1, i-1]_3 * (3^(n-k+i) - 1)/(3^i - 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   gcd_seq, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd_seq.h"
#include "../utils/seq_is_big.h"

static seq_t pow3(idx_t e) {
    seq_t result = 1;
    seq_t base = 3;
    while (e > 0) {
        if (e & 1) result *= base;
        e >>= 1;
        if (e > 0) base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    idx_t k = n / 2;
    if (k == 0) return 1;

    bool big = seq_is_big() != 0;
    idx_t m0 = n - k;
    seq_t pnum = pow3(m0);   /* 3^(n-k+i), starting at i = 0 */
    seq_t pden = 1;          /* 3^i */
    seq_t result = 1;

    for (idx_t i = 1; i <= k; i++) {
        pnum *= 3;
        pden *= 3;
        seq_t num = pnum - 1;
        seq_t den = pden - 1;
        if (big) {
            result = result * num / den;
        } else {
            /* reduce first so the intermediate never exceeds the final value */
            seq_t g = gcd_seq(result, den);
            result /= g;
            den /= g;
            result *= num / den;
        }
    }
    return result;
}
