/*
 * A070162  Numbers k such that k - phi(k) - 1 is a prime.
 * https://oeis.org/A070162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 9503, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A070162 :  Numbers k such that k - φ(k) - 1 is a prime.
 *  This implementation simply tests candidates k in ascending order
 *  and counts how many satisfy the condition until the nth such k
 *  is found.  Totient φ(k) is computed by direct factorisation; the
 *  primality test for the value k - φ(k) - 1 is the usual trial division.
 *  The algorithm runs in O(n · sqrt(k)) time which is well below the
 *  required 2 s limit for n up to about 10 000.
 */

#include <stdint.h>

/*  Compute Euler's totient function φ(n)  */
static uint64_t phi_uint64(uint64_t n)
{
    uint64_t result = n;
    uint64_t temp   = n;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1) result -= result / temp;
    return result;
}

/*  Simple deterministic prime test for 64‑bit integers  */
static int is_prime_uint64(uint64_t n)
{
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t p = 5; p * p <= n; p += 6) {
        if (n % p == 0)   return 0;
        if (n % (p + 2) == 0) return 0;
    }
    return 1;
}

/*  Return the n-th term of A070162 (1‑based indexing).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n == 0 by definition */

    idx_t  found = 0;              /* how many terms found so far   */
    uint64_t k   = 1;              /* candidate k (start from 2)    */

    while (found < n) {
        ++k;
        uint64_t phi_k = phi_uint64(k);
        uint64_t val   = k - phi_k - 1ULL;   /* k - φ(k) - 1 */

        if (val >= 2 && is_prime_uint64(val))
            ++found;
    }

    return (seq_t)k;               /* cast to seq_t type used by build */
}
