/*
 * A005444  From a Fibonacci-like differential equation.
 * https://oeis.org/A005444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} k! * Fibonacci(k+1) * Stirling1(n, k)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t *prev = (seq_t *)calloc((n + 1), sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc((n + 1), sizeof(seq_t));
    
    prev[0] = 1;
    
    for (idx_t i = 1; i <= n; i++) {
        curr[0] = 0;
        for (idx_t j = 1; j <= i; j++) {
            curr[j] = prev[j-1] - (seq_t)(i - 1) * prev[j];
        }
        
        seq_t *temp = prev;
        prev = curr;
        curr = temp;
    }
    
    seq_t result = 0;
    seq_t fact = 1;
    seq_t fib_prev = 0, fib_curr = 1;
    for (idx_t k = 0; k <= n; k++) {
        result += fact * fib_curr * prev[k];
        
        if (k < n) {
            fact *= (k + 1);
            seq_t fib_next = fib_prev + fib_curr;
            fib_prev = fib_curr;
            fib_curr = fib_next;
        }
    }
    
    free(prev);
    free(curr);
    
    return result;
}
