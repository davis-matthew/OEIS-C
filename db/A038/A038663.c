/*
 * A038663  [ n/F_2 ] + [ n/F_3 ] + [ n/F_4 ] +..., F_n=Fibonacci numbers.
 * https://oeis.org/A038663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k>=2} floor(n/F_k), F_1=1, F_2=1,
 *            F_{k+1}=F_k+F_{k-1}
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A038663 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if(n == 0) return (seq_t)0;   /* offset safety, though not needed */

    /* Use seq_t for internal Fibonacci and sum to avoid overflow */
    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;

    seq_t fib_prev = (seq_t)1;   /* F_1 */
    seq_t fib_curr = (seq_t)1;   /* F_2, first term to include in sum */

    while(fib_curr <= n_seq)
    {
        sum += n_seq / fib_curr;     /* floor division */

        /* Next Fibonacci number */
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }

    return sum;
}
