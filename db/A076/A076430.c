/*
 * A076430  Sum of squares of numbers that cannot be written as t*p(n) +
 *          u*p(n+1) for nonnegative integers t,u, where p(n) is the n-th
 *          prime.
 * https://oeis.org/A076430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            p(n)*p(n+1)*(p(n)-1)*(p(n+1)-1)*(p(n)*p(n+1)-p(n)-p(n+1))/12
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* For n < 1 the behavior is undefined; simply return 0 for safety. */
    if (n == 0) return (seq_t)0;
    uint32_t p = get_prime(n);
    uint32_t q = get_prime(n + 1);

    seq_t pseq = (seq_t)p;
    seq_t qseq = (seq_t)q;

    seq_t part1 = pseq * qseq;                         /* p*q */
    seq_t part2 = (pseq - 1) * (qseq - 1);              /* (p-1)*(q-1) */
    seq_t part3 = part1 - pseq - qseq;                 /* p*q - p - q */

    seq_t num  = part1 * part2 * part3;                /* numerator */
    seq_t result = num / (seq_t)12;                    /* divide by 12 */

    return result;
}
