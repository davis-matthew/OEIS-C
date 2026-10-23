/*
 * A151759  G.f.: Theta^3, where Theta = Sum_{k>=0} x^(2^k).
 * https://oeis.org/A151759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (i,j,k) of non‑negative
 *            integers such that 2^i + 2^j + 2^k = n. This implementation
 *            computes the result by iterating over all powers of two not
 *            exceeding n and checking whether the remaining sum is a power of
 *            two
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool is_power_of_two_idx(idx_t x)
{
    return x && ((x & (x - 1)) == 0);
}

seq_t a(idx_t n)
{
    /* No ordered triple can sum to a number smaller than 3. */
    if (n < 3) return 0;

    /* Collect all powers of two not exceeding n. */
    idx_t pow[64];
    int cnt = 0;
    idx_t v = 1;
    while (v <= n) {
        pow[cnt++] = v;
        /* Stop before an overflow that would wrap v to zero. */
        if (v > n >> 1) break;
        v <<= 1;
    }

    seq_t count = 0;

    for (int i = 0; i < cnt; ++i) {
        idx_t pi = pow[i];
        for (int j = 0; j < cnt; ++j) {
            idx_t sum2 = pi + pow[j];
            if (sum2 > n) continue;
            idx_t k = n - sum2;
            if (k >= 1 && is_power_of_two_idx(k)) {
                count += 1;
            }
        }
    }

    return count;
}
