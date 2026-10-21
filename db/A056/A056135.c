/*
 * A056135  Smallest positive integer other than n where geometric mean of n
 *          and a(n) is an integer.
 * https://oeis.org/A056135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{p|n, vp(n) odd} p; If that product equals n (i.e., n
 *            is squarefree or n=1), then a(n)=4*n This is the smallest m≠n
 *            such that sqrt(n·m) is an integer
 * Range:     u8 n <= 64, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for the OEIS sequence A056135 */
seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;      /* input as unsigned 64‑bit */
    uint64_t temp = nn;             /* work copy for factorisation */
    seq_t m0 = (seq_t)1;            /* product of primes with odd exponent */

    /* factor 2 separately to avoid odd loop initialisation */
    int cnt2 = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++cnt2;
    }
    if (cnt2 & 1) m0 *= (seq_t)2ULL;

    /* odd prime factors */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p) continue;
        int cnt = 0;
        while (temp % p == 0) {
            temp /= p;
            ++cnt;
        }
        if (cnt & 1) m0 *= (seq_t)p;
    }

    /* any remaining prime factor (exponent = 1) */
    if (temp > 1) {
        m0 *= (seq_t)temp;
    }

    /* if m0 equals n, the next minimal integer is 4*n */
    if (m0 != nn) {
        return m0;
    }
    return (seq_t)nn * (seq_t)4;
}
