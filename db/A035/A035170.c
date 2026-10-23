/*
 * A035170  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -20.
 * https://oeis.org/A035170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative with * a(2^e)=a(5^e)=1, * a(p^e)=e+1 if p
 *            ≡ 1,3,7,9 (mod 20), * a(p^e)=(1+(-1)^e)/2 if p ≡ 11,13,17,19
 *            (mod 20). * Thus a(n)=∏_{p^e||n} a(p^e). The function computes
 *            a(n) by * factoring n and applying the above rule
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* Handle offset 1: ignore n==0 */
    if (n == 0) return 0;

    uint64_t m = (uint64_t)n;
    /* Prime factorisation: at most 64 distinct primes for 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Primes 2 and 5 contribute a factor of 1 */
        if (p == 2 || p == 5) {
            continue;
        }

        uint64_t mod = p % 20;
        if (mod == 1 || mod == 3 || mod == 7 || mod == 9) {
            result *= (seq_t)(e + 1);
        } else if (mod == 11 || mod == 13 || mod == 17 || mod == 19) {
            /* (1+(-1)^e)/2 == 1 if e even, 0 if e odd */
            seq_t val = (e & 1ULL) ? 0 : 1;
            result *= val;
            if (result == 0) {
                /* Once zero occurs the product stays zero */
                break;
            }
        } else {
            /* Should not occur: all primes other than 2,5 fall into the
             * above residue classes. Keep result unchanged. */
        }
    }
    return result;
}
