/*
 * A094756  a(n) = least k>1 such that (1+2+3+...+k) divides (1^n + 2^n + 3^n
 *          + ... + k^n).
 * https://oeis.org/A094756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>1 such that (1+2+...+k) divides (1^n + 2^n + ...
 *            + k^n)
 * Range:     u8 n <= 9920, u16 n <= 9920, u32 n <= 9920, u64 n <= 9920,
 *            u128 n <= 9920, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple search for k, computing the sum modulo the triangular
   number with fast modular exponentiation.  All arithmetic that may exceed
   seq_t is performed in unsigned 64‑bit integers, with 128‑bit intermediates
   to avoid overflow.  The implementation works for any seq_t (including
   uint8_t) and any idx_t (uint64_t). */

#include <stdint.h>

static unsigned long long modpow_uint64(unsigned long long base,
                                        unsigned long long exp,
                                        unsigned long long mod)
{
    unsigned long long result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Iterate k = 2,3,... until the divisibility holds. */
    for (unsigned long long k = 2ULL; ; ++k) {
        unsigned long long T = k * (k + 1ULL) / 2ULL;   /* S_k */
        unsigned long long sum = 0ULL;
        for (unsigned long long i = 1ULL; i <= k; ++i) {
            sum += modpow_uint64(i, n, T);
            if (sum >= T)   /* keep sum in [0,2T) to reduce integer size */
                sum -= T * (sum / T);
        }
        if (sum % T == 0ULL)
            return (seq_t)k;
    }
}
