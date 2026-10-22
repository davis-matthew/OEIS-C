/*
 * A001690  Non-Fibonacci numbers.
 * https://oeis.org/A001690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) is the n-th positive integer that is not a Fibonacci number
 * Range:     u8 n <= 243, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t fib[100];
    int fib_count = 0;
    
    fib[0] = 1;
    fib[1] = 1;
    fib_count = 2;
    
    while (fib_count < 100) {
        uint64_t next = fib[fib_count - 1] + fib[fib_count - 2];
        if (next < fib[fib_count - 1]) break;
        fib[fib_count] = next;
        fib_count++;
    }
    
    idx_t count = 0;
    uint64_t current = 0;
    
    while (count < n) {
        current++;
        
        int is_fib = 0;
        for (int i = 0; i < fib_count; i++) {
            if (fib[i] == current) {
                is_fib = 1;
                break;
            }
            if (fib[i] > current) break;
        }
        
        if (!is_fib) {
            count++;
        }
    }
    
    return (seq_t)current;
}
