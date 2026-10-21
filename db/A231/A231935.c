/*
 * A231935  Greatest prime Q < 2*n such that 2*n-Q=P prime < Q starting at
 *          n=4.
 * https://oeis.org/A231935
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest prime Q < 2*n such that 2*n - Q is prime * and Q
 *            > (2*n - Q). The sequence starts at n = 4. The function *
 *            returns the value as seq_t. It relies on the helper function *
 *            is_prime(idx_t) from the utils library. For each call the *
 *            function scans odd candidates for Q downwards from...
 * Range:     u8 n <= 129, u16 n <= 9860, u32 n <= 9860, u64 n <= 9860,
 *            u128 n <= 9860, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n < 4) return 0;          /* offset; not specified for n<4 */
    idx_t m = 2 * n;              /* even target for Goldbach pair */

    /* 2*n-1 is always odd. Only odd Q can be prime >2. */
    for (idx_t Q = m - 1; Q > 2; Q -= 2) {
        if (!is_prime(Q)) continue;
        idx_t P = m - Q;
        if (P < Q && is_prime(P)) {
            return (seq_t)Q;     /* greatest Q that satisfies the condition */
        }
    }
    return 0;                     /* should never happen for n >= 4 */
}
