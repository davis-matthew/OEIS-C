/*
 * A049993  a(n) is the number of arithmetic progressions of 3 or more
 *          positive integers, nondecreasing with sum <= n.
 * https://oeis.org/A049993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of arithmetic progressions of length ≥ 3
 * with positive integers, nondecreasing (difference ≥ 0),
 * whose sum is ≤ n.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* no progressions possible */

    seq_t result = 0;
    seq_t n_seq  = (seq_t)n;      /* work in seq_t for safety */

    for (idx_t k = 3; k <= n; ++k) {
        /* T = k*(k-1)/2  – total of the common‑difference part */
        seq_t T = ((seq_t)k * (k - 1)) / 2;

        /* limit = n - k  – the largest value that n - d*T can take
           while still allowing a1 ≥ 1  (i.e. n - d*T ≥ k) */
        seq_t limit = n_seq - (seq_t)k;

        /* maximum admissible d is floor(limit / T)  (may be 0) */
        idx_t max_d = (idx_t)(limit / T);

        for (idx_t d = 0; d <= max_d; ++d) {
            seq_t dT = (seq_t)d * T;
            seq_t rem = n_seq - dT;          /* remaining sum for k*a1 */
            result += rem / (seq_t)k;        /* number of choices for a1 ≥ 1 */
        }
    }
    return result;
}
