/*
 * A099565  Location of records in A099564.
 * https://oeis.org/A099565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A099566(n), where A099566(n) = Product_{k=2..m+1}
 *            F(k) and m is the smallest integer with Sum_{k=1..m} F(k) >= n
 *            (F = Fibonacci numbers)
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t N = (seq_t)n;
    seq_t f1 = 1;        /* F(m)   */
    seq_t f2 = 1;        /* F(m+1) */
    seq_t product = 1;   /* F(2) */
    seq_t sum = 1;       /* F(1) */

    while (N > sum) {
        seq_t f_next = f1 + f2;   /* F(m+2) */
        product *= f2 + f1;       /* include F(m+2)... */
        sum += f2;
        f1 = f2;
        f2 = f_next;
    }
    return N * product;
}
