/*
 * A176835  Number of positive integers k for which k^2 - n*k is a square.
 * https://oeis.org/A176835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9928, u16 n <= 9928, u32 n <= 9928, u64 n <= 9928,
 *            u128 n <= 9928, big any n
 * Helpers:   count_divisor_pairs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A176835
 * a(n) = number of positive integers k for which k^2 – n*k is a square.
 * The implementation counts all divisor pairs (a,b) of n² such that
 *  - a and b have the same parity,
 *  - (a + b + 2*n) is divisible by 4,
 *  - and a ≤ b.  Each such pair corresponds to one valid k.
 * The recursion enumerates the divisors of n² directly from the prime
 * factorisation of n, avoiding any large temporary products.
 *
 * Complexity:  O(number of divisors of n²).
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/count_divisor_pairs.h"

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n=0, but return 0 */

    /* Factorise n: store primes and exponents.  */
    idx_t primes[64];
    idx_t exp_n[64];
    size_t uniq = 0;
    idx_t m = n;
    size_t pos_of_2 = (size_t)-1;
    idx_t e2_n = 0;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[uniq] = p;
            exp_n[uniq] = e;
            if (p == 2) {
                pos_of_2 = uniq;
                e2_n = e;
            }
            ++uniq;
        }
    }
    if (m > 1) {
        primes[uniq] = m;
        exp_n[uniq] = 1;
        if (m == 2) {
            pos_of_2 = uniq;
            e2_n = 1;
        }
        ++uniq;
    }

    /* exp2[i] = 2 * exp_n[i] for n² */
    idx_t exp2[64];
    for (size_t i = 0; i < uniq; ++i)
        exp2[i] = exp_n[i] * 2;

    idx_t cur_exp[64] = {0};

    seq_t count = 0;
    count_divisor_pairs(0, uniq, primes, exp2, cur_exp,
                        n, e2_n, pos_of_2, &count);

    /* Each valid pair (a,b) with a < b is counted twice; a=b (i.e. a=n)
     * is counted once.  Thus correct result is (count + 1) / 2.  */
    return (count + 1) / 2;
}
