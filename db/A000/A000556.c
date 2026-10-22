/*
 * A000556  Expansion of exp(-x) / (1 - exp(x) + exp(-x)).
 * https://oeis.org/A000556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} k!*Fibonacci(k+1)*Stirling2(n,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   stirling2_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/stirling2_row.h"

seq_t a(idx_t n) {
    seq_t *S = stirling2_row(n);
    seq_t result = 0;
    seq_t factorial = 1;
    seq_t fib_prev = 0;
    seq_t fib_curr = 1;
    
    for (idx_t k = 0; k <= n; k++) {
        result += factorial * fib_curr * S[k];
        factorial *= (seq_t)(k + 1);
        
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    free(S);
    return result;
}
