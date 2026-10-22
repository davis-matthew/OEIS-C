/*
 * A006449  Row sums of Fibonacci-Pascal triangle in A045995.
 * https://oeis.org/A006449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=0..n} F(binomial(n,k)), where F = A000045
 *            (Fibonacci numbers)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   fib_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_u64.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    unsigned __int128 c = 1;
    idx_t k;
    for (k = 0; k <= n / 2; k++) {
        seq_t f = fib_u64((uint64_t)c);
        if (k + k == n)
            result += f;
        else
            result += f + f;
        c = c * (unsigned __int128)(n - k) / (unsigned __int128)(k + 1);
    }
    return result;
}
