/*
 * A231563  a(n) = f(1)^n + ... + f(n)^n (mod n) where f(i)=i if gcd(i,n)=1
 *          and f(i)=0 otherwise.
 * https://oeis.org/A231563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( Σ_{i=1}^{n} f(i)^n ) mod n, * where f(i) = i if
 *            gcd(i,n)=1 else 0
 * Range:     u8 n <= 378, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"


seq_t a(idx_t n)
{
    seq_t n_mod = (seq_t)n;
    seq_t sum   = 0;

    for (idx_t i = 1; i <= n; ++i) {
        if (gcd((seq_t)i, n_mod) == 1) {
            seq_t term = mod_pow((seq_t)i, n, n_mod); /* i^n mod n */
            sum += term;
            if (sum >= n_mod)               /* keep sum small */
                sum %= n_mod;
        }
    }
    return sum % n_mod;
}
