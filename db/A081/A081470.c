/*
 * A081470  Smallest product (n+1)(n+2)...(n+k) which is a multiple of n,
 *          where n+k is given by A061243.
 * https://oeis.org/A081470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of consecutive integers from n+1 up to the *
 *            smallest integer m ≥ n for which the product is divisible by n.
 *            * m is characterized by: for every prime p^e dividing n, the
 *            exponent * of p in m! minus the exponent of p in n! is at least
 *            e. * The product is (n+1)(n+2)...(m)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   get_prime_table
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime_table.h"

static idx_t vp_factorial(idx_t n, idx_t p)
{
    idx_t res = 0;
    while (n) {
        n /= p;
        res += n;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1)              /* special case: a(1)=2 */
        return 2;

    const uint32_t *prime_list;
    size_t prime_cnt;
    prime_list = get_prime_table(&prime_cnt);

    idx_t remaining = n;
    idx_t m = n;                     /* minimal upper bound for product */

    for (size_t i = 0; remaining > 1 && i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((idx_t)p * (idx_t)p > remaining) break;

        if (remaining % p == 0) {
            idx_t e = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++e;
            }
            idx_t vp_n = vp_factorial(n, p);
            idx_t cand = n;
            while (vp_factorial(cand, p) - vp_n < e)
                ++cand;
            if (cand > m) m = cand;
        }
    }

    if (remaining > 1) {                 /* remaining prime factor */
        uint32_t p = (uint32_t)remaining;
        idx_t e = 1;
        idx_t vp_n = vp_factorial(n, p);
        idx_t cand = n;
        while (vp_factorial(cand, p) - vp_n < e)
            ++cand;
        if (cand > m) m = cand;
    }

    seq_t prod = 1;
    for (idx_t i = n + 1; i <= m; ++i)
        prod *= (seq_t)i;
    return prod;
}
