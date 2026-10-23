/*
 * A116609  a(n) = 13^n mod n.
 * https://oeis.org/A116609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13^n mod n
 * Range:     u8 n <= 285, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return 13^n modulo n.  For n >= 1.  */
    if (n == 0) return 0;                 /* just in case */
    uint64_t mod  = (uint64_t)n;
    uint64_t base = 13 % mod;             /* 13 mod n */
    uint64_t exp  = (uint64_t)n;
    uint64_t res  = 1 % mod;              /* start with 1 mod n */

    while (exp > 0) {
        if (exp & 1ULL)
            res = (unsigned __int128)res * base % mod;
        exp >>= 1ULL;
        if (exp)
            base = (unsigned __int128)base * base % mod;
    }

    return (seq_t)res;
}
