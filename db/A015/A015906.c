/*
 * A015906  First k>n such that k | n^k + n.
 * https://oeis.org/A015906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = the smallest integer k > n such that k | n^k + n. For odd
 *            n, a(n) = n + 1. For even n, a(n) lies in [n+1, 2n+2] and is
 *            found by checking each candidate k
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n = 0 is a special case: the smallest k > 0 dividing 0 is 1 */
    if (n == 0) return (seq_t)1;

    /* Odd n: trivial by the known property */
    if (n & 1ULL)
        return (seq_t)(n + 1);

    /* Even n: search for the smallest k in [n+1, 2n+2] that satisfies
       k | n^k + n.  All operations are modulo k, so uint64_t is safe
       for n ≤ 10 000 because k ≤ 20 002 and k² < 4 10⁸ < 2³². */
    const idx_t start = n + 1;
    const idx_t end   = 2 * n + 2;   /* inclusive upper bound */

    for (idx_t k = start; k <= end; ++k) {
        /* modular exponentiation: compute n^k mod k */
        uint64_t base = n % k;          /* < k */
        uint64_t exp  = k;              /* exponent */
        uint64_t mod  = k;              /* modulus */
        uint64_t res  = 1 % mod;        /* result so far, modulo k */

        while (exp) {
            if (exp & 1ULL)
                res = (res * base) % mod;
            base = (base * base) % mod;
            exp >>= 1ULL;
        }

        /* Check if k divides n^k + n */
        uint64_t sum = (res + (n % k)) % mod;
        if (sum == 0ULL)
            return (seq_t)k;
    }

    /* Fallback – the bound guarantees a solution, but return end if none found */
    return (seq_t)end;
}
