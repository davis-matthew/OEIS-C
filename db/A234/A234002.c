/*
 * A234002  4n/A234001(n).
 * https://oeis.org/A234002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   * a(n) is the smallest positive integer satisfying the *
 *            constraints in the OEIS entry for A234002: * • for each prime p
 *            with p² | n, p must divide a(n); * • a(n) is a multiple of 4 if
 *            n ≡ 3 (mod 8), * a multiple of 2 if n ≡ 7 (mod 8); * • for each
 *            prime power pᵉ in the factorisation of n (e ≥ 2)...
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    /* Count the exponent of 2 in n and strip it. */
    uint64_t e2 = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0 && tmp != 0) {
        tmp >>= 1;
        ++e2;
    }

    /* Handle the 2‑adic part. */
    if (e2 > 1) {
        if (e2 == 2) {
            result *= (seq_t)4;   /* 2² for e = 2 */
        } else { /* e2 >= 3 */
            seq_t fac = 1;
            for (uint64_t i = 0; i < e2 - 1; ++i)
                fac *= (seq_t)2;
            result *= fac;       /* 2^(e2‑1) for e2 >= 3 */
        }
    }

    /* Factorise the remaining odd part. */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        uint64_t exp = 0;
        while ((tmp % p) == 0) {
            tmp /= p;
            ++exp;
        }
        if (exp >= 2) {
            seq_t fac = 1;
            for (uint64_t i = 0; i < exp - 1; ++i)
                fac *= (seq_t)p;
            result *= fac;       /* p^(exp‑1) for exp ≥ 2 */
        }
    }
    /* Remaining tmp > 1 is a prime with exponent 1 – no extra factor. */

    /* Apply the mod‑8 rules. */
    switch (n & 7) {            /* n % 8 */
        case 3: result *= (seq_t)4; break;   /* multiple of 4 */
        case 7: result *= (seq_t)2; break;   /* multiple of 2 */
        default: break;                       /* no extra factor */
    }

    return result;
}
