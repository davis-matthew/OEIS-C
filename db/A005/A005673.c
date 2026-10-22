/*
 * A005673  a(n) = F(n+2) - 2^[ (n+1)/2 ] - 2^[ n/2 ] + 1.
 * https://oeis.org/A005673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n+2) - 2^floor((n+1)/2) - 2^floor(n/2) + 1
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw2.h"


seq_t a(idx_t n) {
    // Compute F(n+2)
    seq_t fib_prev = 0, fib_curr = 1;
    for (idx_t i = 2; i <= n + 2; i++) {
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    // Result: F(n+2) - 2^floor((n+1)/2) - 2^floor(n/2) + 1
    return fib_curr - pw2((n + 1) / 2) - pw2(n / 2) + 1;
}
