/*
 * A138806  Expansion of (theta_3(q) * theta_3(q^27) + theta_2(q) *
 *          theta_2(q^27) - 1) / 2 in powers of q.
 * https://oeis.org/A138806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 9861, u16 n <= 9861, u32 n <= 9861, u64 n <= 9861,
 *            u128 n <= 9861, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative function for OEIS A138806
 * a(n) = Π f(p^e) over the prime factorisation n=∏p^e
 *    f(2^e)       = 1 if e even, 0 if e odd
 *    f(3^e)       = 0  if e==1,   3  otherwise
 *    f(p^e) (p≡1 mod 6) = e+1
 *    f(p^e) (p≡5 mod 6) = 1 if e even, 0 if e odd
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1)=1 */
    seq_t result = (seq_t)1;

    /* arrays large enough to hold all prime factors of a 64‑bit number */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;

    factor_u64((uint64_t)n, primes, exps, &pcnt);

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t local;

        if (p == 2) {
            /* (1+(-1)^e)/2 ⇒ 1 if e even, 0 if e odd */
            local = (seq_t)(1 - (e & 1u));
        } else if (p == 3) {
            local = (e == 1 ? (seq_t)0 : (seq_t)3);
        } else {
            uint64_t mod = p % 6;
            if (mod == 1) {
                local = (seq_t)(e + 1);
            } else if (mod == 5) {
                local = (seq_t)(1 - (e & 1u));
            } else {
                /* for completeness, although this case never occurs for primes >3 */
                local = (seq_t)1;
            }
        }

        result *= local;
        if (result == 0) break;   /* multiplicative: any zero factor gives zero total */
    }

    return result;
}
