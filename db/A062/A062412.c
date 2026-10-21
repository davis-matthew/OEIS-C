/*
 * A062412  a(n) = Sum_{k=1..n} (k^n + (prime(k) - 1)^n).
 * https://oeis.org/A062412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} (k^n + (prime(k) - 1)^n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   get_prime, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t base1 = (seq_t)k;
        seq_t base2 = (seq_t)get_prime(k) - (seq_t)1;
        sum += pow_int(base1, n);
        sum += pow_int(base2, n);
    }
    return sum;
}
