/*
 * A058161  Number of labeled cyclic groups with a fixed identity.
 * https://oeis.org/A058161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! / φ(n) (OEIS A058161)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

/* Compute gcd for 64‑bit unsigned integers */

/* Euler’s totient function φ(n) for n ≥ 1 */
static uint64_t phi_uint64(idx_t n)
{
    uint64_t result = n;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            result -= result / p;
        }
    }
    if (tmp > 1)
        result -= result / tmp;
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for offset 1, but safe guard */
        return 0;

    uint64_t remaining = phi_uint64(n);   /* φ(n) ≤ n ≤ 10 000  →  fits in uint64_t */
    seq_t   res = 1;

    for (idx_t i = 2; i < n; ++i) {
        uint64_t t = i;
        /* Cancel common factors with the remaining part of φ(n) */
        while (remaining > 1 && t > 1) {
            uint64_t g = gcd_u64(t, remaining);
            if (g == 1) break;
            t      /= g;
            remaining /= g;
        }
        res *= (seq_t)t;
    }
    /* remaining should be 1 here */
    return res;
}
