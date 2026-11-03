/*
 * A236851  Remultiply n first "upward", from GF(2)[X] to N, and then
 *          remultiply that result back "downward", from N to GF(2)[X]: a(n) =
 *          A234741(A234742(n)).
 * https://oeis.org/A236851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A234741(A234742(n))
 * Range:     u8 n <= 64, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 84 terms through n = 8022 (2026-11-01)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/bit_length.h"

/*----- Carry‑less multiplication -------------------------------------*/
static inline seq_t clmul(seq_t a, seq_t b)
{
    seq_t r = 0;
    while (b) {
        if (b & 1) r ^= a;
        a <<= 1;
        b >>= 1;
    }
    return r;
}

/*----- Downward remultiplication: N → GF(2)[X] -------------------*/
static seq_t remultiply_downward(seq_t n)
{
    if (n == 0) return 0;

    /* Factor n into ordinary primes.  For the small values of n appearing
       from a(n) (for n up to 10000) a simple trial division is ample. */
    seq_t res = 1;
    seq_t m = n;

    /* trial division by 2 */
    while ((m & 1) == 0) {
        res = clmul(res, (seq_t)2);
        m >>= 1;
    }
    /* trial division by odd numbers up to sqrt(m) */
    for (uint64_t p = 3; (seq_t)p * (seq_t)p <= m; p += 2) {
        while (m % (seq_t)p == 0) {
            res = clmul(res, (seq_t)p);
            m /= (seq_t)p;
        }
    }
    /* any remaining prime factor > sqrt(m) */
    if (m > 1) res = clmul(res, m);

    return res;
}

/*----- Division in GF(2)[X], returns quotient via pointer ----------------*/
static inline bool poly_divides(seq_t p, seq_t d, seq_t *quot)
{
    if (d == 0) return false;
    seq_t rem = p;
    *quot = 0;
    unsigned bd = bit_length(d);
    while (rem && bit_length(rem) >= bd) {
        unsigned shift = bit_length(rem) - bd;
        rem ^= d << shift;
        *quot |= ((seq_t)1 << shift);
    }
    return rem == 0;
}

/*----- Recursive factorisation of a polynomial over GF(2) ------------
   Returns the ordinary product of the integer representations of
   irreducible factors. -------------------------------------------------*/
static seq_t factor_product(seq_t p)
{
    if (p <= 1) return p;   /* 0 or 1 are treated as themselves */

    unsigned deg_p = bit_length(p) - 1;
    for (unsigned d_deg = 1; d_deg <= deg_p / 2; ++d_deg) {
        seq_t base = ((seq_t)1 << d_deg);          /* monic part */
        seq_t limit = ((seq_t)1 << d_deg) - 1;     /* lower bits */
        for (seq_t mask = 0; mask <= limit; ++mask) {
            seq_t d = base | mask;                 /* candidate divisor */
            seq_t q;
            if (poly_divides(p, d, &q)) {
                seq_t left  = factor_product(d);
                seq_t right = factor_product(q);
                return left * right;
            }
        }
    }
    /* irreducible polynomial */
    return p;
}

/*----- Upward remultiplication: GF(2)[X] → N ---------------------*/
static seq_t remultiply_upward(seq_t n)
{
    if (n == 0) return 0;
    return factor_product(n);
}

/*----- Entry point ----------------------------------------------------*/
seq_t a(idx_t n)
{
    seq_t up = remultiply_upward((seq_t)n);
    return remultiply_downward(up);
}
