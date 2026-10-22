/*
 * A002586  Smallest prime factor of 2^n + 1.
 * https://oeis.org/A002586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 3, 5, 3, 17 for n == 1, 2, 3, 4 (mod 8); otherwise
 *            search: the smallest p == 1 (mod 2^(v2(n)+2)) with 2^n == -1
 *            (mod p)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 127, u64 n <= 3967,
 *            u128 n <= 3967, big any n
 * Helpers:   a2586_mulmod, a2586_powmod2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a2586_mulmod.h"
#include "../utils/a2586_powmod2.h"

seq_t a(idx_t n) {
    uint64_t k = 0, m = n, step, p, budget;
    if (n & 1) return (seq_t)3;
    if ((n & 3) == 2) return (seq_t)5;
    if ((n & 7) == 4) return (seq_t)17;
    while ((m & 1) == 0) { m >>= 1; k++; }
    if (k + 2 >= 62) return (seq_t)0;
    /* every prime factor p of 2^n+1 has 2 of order 2^(k+1)*d, and p == 1 (mod 8),
       so 2 is a QR and p == 1 (mod 2^(k+2)). The smallest such p dividing
       2^n+1 is prime: any composite one has smaller qualifying prime factors. */
    step = (uint64_t)1 << (k + 2);
    budget = 40000000;
    for (p = step + 1; p < ((uint64_t)1 << 62) && budget > 0; p += step, budget--) {
        if (p % 3 == 0 || p % 5 == 0 || p % 7 == 0) continue;
        if (a2586_powmod2(n, p) == p - 1) return (seq_t)p;
    }
    return (seq_t)0;
}
