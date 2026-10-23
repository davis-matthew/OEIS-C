/*
 * A103433  a(n) = Sum_{i=1..n} Fibonacci(2i-1)^2.
 * https://oeis.org/A103433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} Fibonacci(2*i-1)^2 Recurrence: let (a_k,
 *            b_k) = (F(odd_k), F(odd_k+1)) with odd_k = 2*i-1 Then next
 *            odd_k+2: a_{k+1} = F(odd_k+2) = a_k + b_k b_{k+1} = F(odd_k+3) =
 *            b_k + a_{k+1} The sequence can be accumulated iteratively in
 *            O(n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    /* Starting values: F(1)=1, F(2)=1 */
    seq_t a_k = 1;   /* F(2*i-1) for current i */
    seq_t b_k = 1;   /* F(2*i)   for current i */

    /* Handle n==0 separately: sum already 0. */
    for (idx_t i = 1; i <= n; ++i) {
        /* sum += a_k * a_k;   compute square */
        sum += a_k * a_k;

        if (i == n) break;  /* avoid one extra iteration */

        /* advance to next odd Fibonacci number */
        seq_t a_next = a_k + b_k;          /* F(odd+2) */
        b_k = b_k + a_next;                /* F(odd+3) */
        a_k = a_next;                      /* set new odd */
    }

    return sum;
}
