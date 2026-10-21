/*
 * A085314  Number of distinct 11th powers modulo n.
 * https://oeis.org/A085314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ul_gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of distinct 11th power residues modulo n.
   The sequence is multiplicative.  For each prime power p^e dividing n
   we compute the number of residues modulo that power:

   * if p == 2   (k = 11 is odd) :   res = 1 + φ(2^e)            (φ(2^e)=2^{e-1})
   * if p  > 2   :   res = 1 + φ(p^e) / gcd(11, φ(p^e))
   The overall term a(n) is the product of these contributions. */

#include <stdint.h>
#include "../utils/ul_gcd64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* n=0 not used, safety */
    if (n == 1) return 1;              /* only residue 0 */

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* factor 2 separately */
    int e = 0;
    while ((m & 1ULL) == 0) {
        ++e;
        m >>= 1;
    }
    if (e > 0) {
        /* φ(2^e) = 2^{e-1} */
        seq_t phi = (seq_t)1 << (e - 1);      /* works for e>=1 */
        seq_t contrib = (seq_t)1 + phi;
        result *= contrib;
    }

    /* odd prime factors */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            /* φ(p^e) = p^{e-1} * (p-1) */
            seq_t phi = (seq_t)1;
            uint64_t phi64 = 1;
            for (int i = 1; i < e; ++i) {
                phi *= (seq_t)p;
                phi64 *= (uint64_t)p;
            }
            phi *= (seq_t)(p - 1);
            phi64 *= (uint64_t)(p - 1);

            uint64_t g = ul_gcd64((uint64_t)11, phi64);   /* gcd(11, φ) */
            seq_t contrib = (seq_t)1 + phi / (seq_t)g;
            result *= contrib;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1) {
        idx_t p = m;                       /* exponent e = 1 */
        seq_t phi = (seq_t)(p - 1);
        uint64_t phi64 = (uint64_t)(p - 1);
        uint64_t g = ul_gcd64((uint64_t)11, phi64);
        seq_t contrib = (seq_t)1 + phi / (seq_t)g;
        result *= contrib;
    }

    return result;
}
