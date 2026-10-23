/*
 * A180783  Number of distinct solutions of Sum_{i=1..1} (x(2i-1)*x(2i)) == 1
 *          (mod n), with x() in {1,2,...,n-1}.
 * https://oeis.org/A180783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n=1, otherwise a(n) = (phi(n) + A060594(n)) / 2
 *            where A060594(n) is the number of solutions of x^2 ≡ 1 (mod n)
 * Range:     u8 n <= 329, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return 0;
    }

    /* Compute phi(n) */
    seq_t phi_val = phi(n);

    /* Factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    /* Count solutions to x^2 ≡ 1 (mod n) */
    uint64_t sol = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t mult = 0;
        if (p == 2) {
            if (e == 1)       mult = 1;  /* mod 2 */
            else if (e == 2)  mult = 2;  /* mod 4 */
            else              mult = 4;  /* mod 2^e with e>=3 */
        } else {
            mult = 2;          /* odd prime power */
        }
        sol *= mult;
    }

    /* a(n) = (phi(n) + sol) / 2 */
    seq_t sum = phi_val + (seq_t)sol;
    seq_t res = sum / 2;
    return res;
}
