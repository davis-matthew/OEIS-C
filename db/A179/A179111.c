/*
 * A179111  Partial sums of round(Fibonacci(n)/11).
 * https://oeis.org/A179111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-10) + Fib(n-3) - 1, for n > 10; initial values:
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1, a(7)=2,
 *            a(8)=4, a(9)=7, a(10)=12
 * Range:     u8 n <= 16, u16 n <= 27, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    /* Zero to ten here are returned literally.  For larger indices
       we use the recurrence a(n)=a(n-10)+Fib(n-3)-1. */
    if (n <= 10) {
        switch (n) {
            case 0: return (seq_t)0;
            case 1: return (seq_t)0;
            case 2: return (seq_t)0;
            case 3: return (seq_t)0;
            case 4: return (seq_t)0;
            case 5: return (seq_t)0;
            case 6: return (seq_t)1;
            case 7: return (seq_t)2;
            case 8: return (seq_t)4;
            case 9: return (seq_t)7;
            case 10: return (seq_t)12;
        }
    }
    return a(n - 10) + fib(n - 3) - (seq_t)1;
}
