/*
 * A225658  a(n) = n! + (n+1)! + 3*(n+2)!.
 * https://oeis.org/A225658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! + (n+1)! + 3*(n+2)!
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    // Compute n! using the utilities.
    seq_t fact_n = factorial(n);

    // (n+1)! = n! * (n+1)
    seq_t n1 = (seq_t)(n + 1);
    seq_t fact_n1 = fact_n * n1;

    // (n+2)! = (n+1)! * (n+2)
    seq_t n2 = (seq_t)(n + 2);
    seq_t fact_n2 = fact_n1 * n2;

    // a(n) = n! + (n+1)! + 3*(n+2)!
    return fact_n + fact_n1 + (seq_t)3 * fact_n2;
}
