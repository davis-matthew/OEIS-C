/*
 * A070238  Sign of core(n)-phi(n) where core(n) is the squarefree part of n
 *          and phi the Euler totient function.
 * https://oeis.org/A070238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = sign(core(n) – φ(n)) (core = squarefree part of n)
 * Range:     i8 n <= 215, i16 n <= 9657, i32 n <= 9657, i64 n <= 9657,
 *            i128 n <= 9657, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is at least 1 by the OEIS offset */
    if (n == 1) {
        /* core(1) = φ(1) = 1, so the sign is 0 */
        return 0;
    }

    seq_t core = (seq_t)1;           /* product of odd‑power prime factors */
    seq_t phi  = (seq_t)n;           /* Euler’s totient, will be reduced */

    uint64_t m = (uint64_t)n;        /* work in 64‑bit unsigned form */

    /* Trial division up to sqrt(m).  For all practical n in this
       sequence (typically ≤ 10^4) this is fast enough. */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            /* core gets the prime factor only when its exponent is odd */
            if (exp & 1) {
                core *= (seq_t)p;
            }
            /* update φ:  φ = φ * (1 – 1/p)  →  φ -= φ/p  */
            phi -= phi / (seq_t)p;
        }
    }

    /* If m > 1 it is a prime factor left with exponent 1. */
    if (m > 1) {
        core *= (seq_t)m;          /* exponent is odd (1) */
        phi -= phi / (seq_t)m;
    }

    if (core > phi) return 1;
    if (core < phi) return -1;
    return 0;                     /* equality can only occur for n = 1 */
}
