/*
 * A070964  a(n) = Sum_{1<=k<=n, gcd(k,n)=1} Fibonacci(k).
 * https://oeis.org/A070964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{1<=k<=n, gcd(k,n)=1} Fibonacci(k)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 46, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   fib, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/fib.h"

seq_t a(idx_t n) {
    /* Simple loop over all k <= n, using the provided gcd and fib functions.
       For small n (up to about 10 000) this is fast enough. */
    seq_t sum = 0;
    seq_t sn = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd((seq_t)k, sn) == 1) {
            sum += fib(k);
        }
    }
    return sum;
}
