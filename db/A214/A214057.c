/*
 * A214057  Least m>0 such that 2^n-1+m and n-m have a common divisor > 1.
 * https://oeis.org/A214057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m>0 such that gcd(2^n - 1 + m, n - m) > 1
 * Range:     u8 n <= 265, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset undefined, but safe */

    seq_t two = 2;
    seq_t def = (seq_t)n;            /* return n if nothing is found */

    /* iterate B = n-1 ... 1  (B = n-m) */
    for (idx_t B = n - 1; B > 0; --B) {
        seq_t mod = (seq_t)B;
        seq_t p   = mod_pow(two, n, mod);         /* 2^n mod B */
        seq_t T   = (p + (seq_t)(n - 1)) % mod;   /* (2^n + n - 1) mod B */

        if (gcd(T, mod) > 1) {                  /* common divisor > 1 */
            def = (seq_t)(n - B);                /* m = n - B */
            break;
        }
    }
    return def;
}
