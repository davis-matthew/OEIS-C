/*
 * A069254  k/10 when gcd(k, 2^k+1) = 5.
 * https://oeis.org/A069254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A069254
 *  a(n) is the n-th integer m such that
 *  gcd(10*m, 2^(10*m)+1) = 5.  The sequence values are
 *  a(n) = m (i.e. k/10 where k=10m satisfies the condition).   */

#include <stdint.h>

/*  Compute 2^exp mod mod using binary exponentiation.  */
static inline uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

/*  Euclidean greatest‑common‑divisor for 64‑bit integers.  */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1, not defined for n=0 */

    idx_t m = 1;                       /* candidate m = k/10  */
    idx_t count = 0;                   /* how many valid ks found */

    while (1) {
        uint64_t k = 10ULL * m;        /* k = 10*m  */
        /* compute 2^k mod k  */
        uint64_t r = modpow_uint64(2ULL, k, k);
        uint64_t val = (r + 1ULL) % k;  /* (2^k + 1) mod k  */
        uint64_t g = gcd_uint64(k, val);

        if (g == 5ULL) {
            count++;
            if (count == n)
                return (seq_t)m;       /* a(n) = m  */
        }
        m++;
    }
}
