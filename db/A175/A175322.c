/*
 * A175322  a(n) = A053141(n)*A001109(n+1) =
 *          Sum_{k=A053141(n)+1..A001109(n+1)-1} k.
 * https://oeis.org/A175322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 41*a(n-1) - 246*a(n-2) + 246*a(n-3) - 41*a(n-4) + a(n-5)
 *            * with initial values * a(1)=12, a(2)=490, a(3)=17136,
 *            a(4)=584988, a(5)=19889100
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)490;
    if (n == 3) return (seq_t)17136;
    if (n == 4) return (seq_t)584988;
    if (n == 5) return (seq_t)19889100;

    seq_t t1 = (seq_t)12;
    seq_t t2 = (seq_t)490;
    seq_t t3 = (seq_t)17136;
    seq_t t4 = (seq_t)584988;
    seq_t t5 = (seq_t)19889100;

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 41 * t5 - 246 * t4 + 246 * t3 - 41 * t2 + t1;
        t1 = t2; t2 = t3; t3 = t4; t4 = t5; t5 = next;
    }
    return t5;
}
