/*
 * A050468  a(n) = Sum_{d|n, n/d=1 mod 4} d^4 - Sum_{d|n, n/d=3 mod 4} d^4.
 * https://oeis.org/A050468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, changed
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A050468: a(n) = Sum_{d|n, n/d≡1 (mod 4)} d^4 – Sum_{d|n, n/d≡3 (mod 4)} d^4
   The sequence is multiplicative.
   • a(2^e) = 16^e
   • If p≡1 (mod 4): a(p^e) = 1 + p^4 + p^8 + … + p^{4e}
   • If p≡3 (mod 4): a(p^e) = ∑_{j=0}^{e} (-1)^{e-j} (p^4)^j
   The function factors n, builds the local factors for each prime power,
   and multiplies them together to obtain a(n).  All arithmetic is done with
   seq_t to support arbitrary‑precision values.  The routine works in
   O(Ω(n)) time for each call and does not depend on the order of calls. */
#include <stdint.h>
#include <stddef.h>
/* Helper functions provided by the build environment */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/pow_uint.h"    /* seq_t pow_uint(uint64_t base, idx_t exp); */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int      e = (int)exps[i];          /* exponent of p in n */

        seq_t contrib;

        if (p == 2U) {
            /* a(2^e) = 16^e */
            contrib = pow_uint(16ULL, (idx_t)e);
        } else {
            /* Compute p^4 as a seq_t */
            seq_t x = pow_uint(p, 4ULL);   /* x = p^4 */

            if (p % 4ULL == 1ULL) {
                /* a(p^e) = 1 + x + x^2 + … + x^e  (geometric series) */
                seq_t term = (seq_t)1;
                seq_t sum  = (seq_t)0;
                for (int j = 0; j <= e; ++j) {
                    sum  += term;
                    term *= x;                 /* term = x^{j+1} for next iteration */
                }
                contrib = sum;
            } else { /* p % 4 == 3 */
                /* a(p^e) = ∑_{j=0}^{e} (-1)^{e-j} x^j  */
                seq_t term = (seq_t)1;
                seq_t pos  = (seq_t)0;
                seq_t neg  = (seq_t)0;
                for (int j = 0; j <= e; ++j) {
                    /* sign = (-1)^{e-j} : even -> positive, odd -> negative */
                    if (((e - j) & 1) == 0)
                        pos += term;
                    else
                        neg += term;
                    term *= x;                            /* next power of x */
                }
                /* pos >= neg for all e, so the subtraction is safe */
                contrib = pos - neg;
            }
        }

        result *= contrib;
    }

    return result;
}
