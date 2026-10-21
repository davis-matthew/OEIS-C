/*
 * A226240  Expansion of phi(q^4) * phi(q^8) + 2 * q *phi(q^2) * psi(q^8) in
 *          powers of q where phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A226240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A226240: multiplicative formula  
   a(0) = 1
   For n > 0, let n = 2^e2 * ∏ p_i^e_i (p_i odd primes).
   Define b(2^e) = 0 if e == 1, 1 if e > 1, and
          b(p^e) = e + 1       if p ≡ 1 or 3 (mod 8),
          b(p^e) = 0           if e odd and p ≡ 5 or 7 (mod 8),
          b(p^e) = 1           if e even and p ≡ 5 or 7 (mod 8).
   Then a(n) = 2 * ∏ b(p_i^e_i), with the obvious convention that the
   empty product equals 1.  This implements the definition directly.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the framework */

static inline seq_t factor_value(idx_t p, idx_t e)
{
    /* p is odd except possibly p==2 handled below */
    if (p == 2ULL) {
        return (e == 1ULL) ? 0 : 1;
    }
    idx_t mod8 = p & 7ULL;          /* p % 8 */
    if (mod8 == 1ULL || mod8 == 3ULL) {
        return (seq_t)(e + 1ULL);
    } else { /* mod8 == 5 or 7 */
        return ((e & 1ULL) == 0ULL) ? 1 : 0;
    }
}

seq_t a(idx_t n)
{
    if (n == 0ULL) return 1;

    seq_t prod = 1;
    idx_t m = n;

    /* factor out powers of 2 */
    idx_t e2 = 0;
    while ((m & 1ULL) == 0ULL) {
        e2++;
        m >>= 1;
    }
    if (e2 == 1ULL) return 0;          /* b(2) = 0 kills the product */

    /* odd prime factors */
    if (m > 1ULL) {
        /* trial division, skip even numbers */
        for (idx_t p = 3ULL; p <= m / p; p += 2ULL) {
            if (m % p == 0ULL) {
                idx_t e = 0;
                while (m % p == 0ULL) {
                    m /= p;
                    e++;
                }
                seq_t f = factor_value(p, e);
                if (f == 0) return 0;
                prod *= f;
            }
        }
        if (m > 1ULL) {                /* remaining prime factor */
            seq_t f = factor_value(m, 1ULL);
            if (f == 0) return 0;
            prod *= f;
        }
    }

    return prod * 2;   /* a(n) = 2 * b(n) for n > 0 */
}
