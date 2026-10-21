/*
 * A211416  Number of self-inverse 2 X 2 matrices with all terms in
 *          {-n,...,0,...,n}.
 * https://oeis.org/A211416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 (for n ≥ 1) + Σ_{a=-n}^{n} countPairs(1 - a² , n) *
 *            where countPairs(r, n) is the number of ordered pairs (b, c)
 *            with * b, c ∈ { -n, …, 0, …, n } and b·c = r
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   count_pairs_signed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation works for all integer types seq_t and idx_t. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/count_pairs_signed.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 2;                     /* Case 2: a = d = ±1 */

    idx_t n_lim = n;
    for (idx_t a = 0; a <= n_lim; ++a) {
        int64_t a_int = (int64_t)a;
        int64_t r     = 1 - a_int * a_int;          /* r = 1 - a² */
        seq_t  cnt    = count_pairs_signed(r, n_lim);
        if (a == 0) {
            result += cnt;
        } else {
            result += cnt << 1;                    /* multiply by 2 for ±a */
        }
    }
    return result;
}
