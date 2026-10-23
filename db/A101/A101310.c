/*
 * A101310  Sum(prime(k)*10^(k),k=1..n).
 * https://oeis.org/A101310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1}^{n} prime(k) * 10^k , a(0)=0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* static storage for a small sieve (large enough for n=10000) */
    static bool sieve_done = false;
    enum { MAX_VAL = 200000 };               /* > 104729 (10000th prime) */
    static bool marked[MAX_VAL + 1];
    static uint32_t primes[MAX_VAL];
    static idx_t num_primes = 0;

    if (!sieve_done) {
        num_primes = 0;
        for (idx_t i = 2; i <= MAX_VAL; ++i) {
            if (!marked[i]) {
                primes[num_primes++] = (uint32_t)i;
                if (i * i <= MAX_VAL) {
                    for (idx_t j = i * i; j <= MAX_VAL; j += i)
                        marked[j] = true;
                }
            }
        }
        sieve_done = true;
    }

    seq_t sum = 0;
    seq_t pow10 = 1; /* 10^0 */
    for (idx_t k = 1; k <= n; ++k) {
        pow10 *= 10; /* now pow10 = 10^k */
        sum += (seq_t)primes[(size_t)(k - 1)] * pow10;
    }
    return sum;
}
