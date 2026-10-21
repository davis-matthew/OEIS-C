/*
 * A052983  Least multiple of n consisting of a succession of 1's followed by
 *          a succession of 0's.
 * https://oeis.org/A052983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest integer of the form * ((10^s – 1)/9) * 10^t
 *            that is divisible by n * where t = max(v₂(n), v₅(n)) (and t = 1
 *            if t = 0), * and s is the smallest positive integer such that *
 *            n / 2ⁿ·5ⁿ | (10^s – 1)/9. * This is equivalent to finding the
 *            multiplicative order of 10 modulo 9m, * whe...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 22, u128 n <= 27,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Separate powers of 2 and 5 from n. */
    idx_t m = n;
    idx_t p = 0, q = 0;
    while ((m & 1ULL) == 0ULL) { ++p; m >>= 1; }
    while (m % 5ULL == 0ULL)   { ++q; m /= 5;   }

    /* The number of trailing zeros t.  It must be at least 1. */
    idx_t t = (p > q) ? p : q;
    if (t == 0) t = 1;

    /* Find minimal s with 10^s ≡ 1 (mod 9m). */
    uint64_t mod = 9ULL * m;          /* m < 10⁵ for the intended limits */
    uint64_t cur = 10ULL % mod;       /* current 10^k mod mod */
    idx_t    s  = 1;
    while (cur != 1ULL) {
        cur = (unsigned __int128)cur * 10ULL % mod;
        ++s;
    }

    /* Compute the required value. */
    seq_t power_t = pow_uint(10, t);
    seq_t power_s = pow_uint(10, s);
    seq_t repunit = (power_s - 1) / 9;   /* integer division is exact */
    return power_t * repunit;
}
