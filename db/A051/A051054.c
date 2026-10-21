/*
 * A051054  a(n) = Sum_{k=1..n} C(n, floor(n/k)).
 * https://oeis.org/A051054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} C(n, floor(n/k))
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t my_binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t num_idx = n - k + i;
        idx_t den_idx = i;
        seq_t num = (seq_t)num_idx;
        seq_t den = (seq_t)den_idx;

        /* Cancel common factors between result and den */
        seq_t g1 = gcd(result, den);
        result /= g1;
        den /= g1;

        /* Cancel common factors between num and den */
        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;

        if (sizeof(seq_t) <= 8) {
            /* Use 128‑bit arithmetic to avoid overflow */
            unsigned __int128 temp = (unsigned __int128)result *
                                    (unsigned __int128)num;
            result = (seq_t)(temp / den);
        } else {
            /* seq_t is an arbitrary‑precision type */
            result = (seq_t)((result * num) / den);
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k = 1;
    while (k <= n) {
        idx_t m = n / k;                     /* floor(n/k) */
        idx_t next_k = n / m + 1;             /* first k where floor(n/k) < m */
        idx_t cnt = next_k - k;               /* number of k that give this m */
        seq_t bin = my_binom(n, m);           /* C(n,m) */
        sum += bin * (seq_t)cnt;
        k = next_k;
    }
    return sum;
}
