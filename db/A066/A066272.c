/*
 * A066272  Number of anti-divisors of n.
 * https://oeis.org/A066272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000005(2*n-1) + A000005(2*n+1) + A001227(n) - 5 for n ≥
 *            2; a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_idx.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }

    idx_t two_n_minus1 = n + n - 1;
    idx_t two_n_plus1  = n + n + 1;

    seq_t d1 = divisor_count_idx(two_n_minus1);
    seq_t d2 = divisor_count_idx(two_n_plus1);

    /* Odd divisors of n: remove all factors of 2. */
    idx_t n_odd = n;
    while ((n_odd & 1ULL) == 0) {
        n_odd >>= 1;
    }
    seq_t odd_cnt = divisor_count_idx(n_odd);

    seq_t result = d1 + d2 + odd_cnt;
    result -= 5;   /* result is non‑negative for n ≥ 2 */

    return result;
}
