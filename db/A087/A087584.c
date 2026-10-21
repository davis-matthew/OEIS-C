/*
 * A087584  a(n) = (1/7)*Sum_{k=0..n} binomial(n,k)*Fibonacci(k)*7^k.
 * https://oeis.org/A087584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=9, a(n)=9*a(n-1)+41*a(n-2)  (OEIS A087584) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)9;
    seq_t an = 0;
    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * ((seq_t)9) + a0 * ((seq_t)41);
        a0 = a1;
        a1 = an;
    }
    return an;
}
