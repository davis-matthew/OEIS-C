/*
 * A031361  Number of symmetrically inequivalent coincidence rotations of
 *          index n in lattice Z^4.
 * https://oeis.org/A031361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) is multiplicative. * a(p^k) for odd prime p: * a(p^0) = 1 *
 *            a(p^1) = (p+1)^2 * a(p^2) = p + (p+p^2)*(p+1)^2 - p^3 * a(p^k) =
 *            (p+p^2)*a(p^{k-1}) - p^3*a(p^{k-2}) for k ≥ 3 * For p = 2: *
 *            a(2^0) = 1 * a(2^1) = 2 * a(2^k) = 0 (k ≥ 2) * Thus a(n)=0 iff 4
 *            divides n. For n = 2^e * ∏_{p odd} p^{k_p...
 * Range:     u8 n <= 14, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a_p_pow_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a_p_pow_odd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* sequence starts at index 1, undefined here */

    /* Handle factor 2 */
    idx_t exp2 = 0;
    while ((n % 2) == 0) {
        ++exp2;
        n /= 2;
    }
    if (exp2 >= 2)          /* 4 divides original n */
        return 0;
    seq_t result = 1;
    if (exp2 == 1)
        result = 2;         /* factor a(2) */

    /* Factor odd part */
    for (uint64_t p = 3; (uint64_t)p * (uint64_t)p <= n; p += 2) {
        if (n % p == 0) {
            idx_t e = 0;
            while (n % p == 0) {
                ++e;
                n /= p;
            }
            seq_t apow = a_p_pow_odd(p, e);
            result *= apow;
        }
    }
    /* Remaining prime > sqrt(original n) */
    if (n > 1) {
        seq_t apow = a_p_pow_odd(n, 1);  /* exponent 1 */
        result *= apow;
    }

    return result;
}
