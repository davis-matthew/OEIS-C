/*
 * A024928  a(n) = Sum_{k=1..n} floor( (n + prime(k))/k ).
 * https://oeis.org/A024928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor( (n + prime(k))/k )
 * Range:     u8 n <= 36, u16 n <= 3930, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset starts at 1, guard for n=0 */

    seq_t sum = 0;
    idx_t N = n;

    for (idx_t k = 1; k <= N; ++k) {
        seq_t pk = (seq_t)get_prime(k);          /* prime(k) */
        seq_t num = (seq_t)N + pk;               /* n + prime(k) */
        seq_t term = num / (seq_t)k;             /* floor division */
        sum += term;                             /* accumulate */
    }

    return sum;
}
