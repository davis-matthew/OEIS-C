/*
 * A234001  Lowest common modulus to which the set of residue classes (mod 4n)
 *          that all the primes represented by a certain quadratic form of
 *          discriminant = -4n belong to, can be simplified to, for all
 *          quadratic forms of discriminant = -4n.
 * https://oeis.org/A234001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   let n = 2^e * m with m odd. P = product of the distinct odd
 *            prime factors of m (P=1 if m=1). t = 2‑part multiplier
 *            determined by e and m mod 8: if e == 0 (n is odd) if m mod 8 is
 *            1 or 5 -> t = 4 if m mod 8 is 3 -> t = 1 if m mod 8 is 7 -> t =
 *            2 else if e == 1 -> t = 8 else if e == 2 -> t = 4 else...
 * Range:     u8 n <= 64, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

seq_t a(idx_t n)
{
    /* split n into 2^e * m, with m odd */
    idx_t e = 0;
    while ((n & 1ULL) == 0 && n != 0) {
        e++;
        n >>= 1;
    }
    idx_t m = n;          /* now odd (m==0 only if original n==0, ignore) */

    /* product of distinct odd primes dividing m */
    seq_t P = (seq_t)1;
    if (m > 1) {
        /* factor_m stores primes and exponents */
        uint64_t prs[64];
        uint64_t exs[64];
        size_t cnt = 0;
        factor_u64(m, prs, exs, &cnt);
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = prs[i];
            if (p > 2) {
                P *= (seq_t)p;
            }
        }
    }

    /* determine t */
    seq_t t = (seq_t)1;
    if (e == 0) {                       /* n is odd */
        uint8_t mod8 = (uint8_t)(m % 8ULL);
        if (mod8 == 1 || mod8 == 5) {
            t = (seq_t)4;
        } else if (mod8 == 3) {
            t = (seq_t)1;
        } else { /* mod8 == 7 */
            t = (seq_t)2;
        }
    } else if (e == 1) {
        t = (seq_t)8;
    } else if (e == 2) {
        t = (seq_t)4;
    } else { /* e >= 3 */
        t = (seq_t)8;
    }

    return P * t;
}
