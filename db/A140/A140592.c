/*
 * A140592  a(n) = 2n if A010060(n-1) is 0, and a(n) = 2n+1 if A010060(n-1) is
 *          1.
 * https://oeis.org/A140592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 2*n + (popcount(n-1) mod 2)
 * Range:     u8 n <= 127, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Equivalent to a(n) = 2n if the Thue-Morse value A010060(n-1) is 0,
 *   else a(n) = 2n+1. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute parity of popcount(n-1). */
    idx_t m = n - 1;
    bool parity = false;
    while (m) {
        parity ^= (m & 1);
        m >>= 1;
    }

    /* 2*n + parity (0 or 1). */
    seq_t result = (seq_t)(n << 1);
    result += parity;
    return result;
}
