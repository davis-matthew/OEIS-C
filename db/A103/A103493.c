/*
 * A103493  Multiplicative suborder of 7 (mod n) = sord(7, n).
 * https://oeis.org/A103493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   modpow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/**
 * Compute the multiplicative suborder of 7 (mod n):
 *  a(n) = minimum e for which 7^e = +/-1 (mod n),
 *  or 0 if no such e exists.
 */
#include <stdint.h>
#include "../utils/modpow.h"

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;

    if (nn <= 1)          /* a(0) = a(1) = 0 */
        return (seq_t)0;

    if (nn % 7ULL == 0)   /* if 7|n then no e exists */
        return (seq_t)0;

    uint64_t r = 7ULL % nn;
    if (r == 1ULL || r == nn - 1ULL)   /* 7 ≡ ±1 (mod n) -> e = 1 */
        return (seq_t)1;

    /* compute Euler's totient φ(n) */
    uint64_t temp = nn;
    uint64_t phi = nn;
    for (uint64_t p = 2ULL; p * p <= temp; ++p) {
        if (temp % p == 0ULL) {
            phi -= phi / p;
            while (temp % p == 0ULL)
                temp /= p;
        }
    }
    if (temp > 1ULL)          /* remaining prime factor */
        phi -= phi / temp;

    /* factor φ(n) to compute the multiplicative order of 7 modulo n */
    uint64_t phi_copy = phi;
    uint64_t primes[64];
    int      exps[64];
    int       count = 0;
    for (uint64_t p = 2ULL; p * p <= phi_copy; ++p) {
        if (phi_copy % p == 0ULL) {
            int e = 0;
            while (phi_copy % p == 0ULL) {
                phi_copy /= p;
                ++e;
            }
            primes[count] = p;
            exps[count]   = e;
            ++count;
        }
    }
    if (phi_copy > 1ULL) {
        primes[count] = phi_copy;
        exps[count]   = 1;
        ++count;
    }

    uint64_t order = phi;
    for (int i = 0; i < count; ++i) {
        uint64_t d = primes[i];
        while (order % d == 0ULL) {
            uint64_t candidate = order / d;
            if (modpow(7ULL, candidate, nn) == 1ULL)
                order = candidate;
            else
                break;
        }
    }

    /* check if order/2 gives -1 */
    if (order % 2ULL == 0ULL) {
        uint64_t half = order / 2ULL;
        if (modpow(7ULL, half, nn) == nn - 1ULL)
            return (seq_t)half;
    }

    return (seq_t)order;
}
