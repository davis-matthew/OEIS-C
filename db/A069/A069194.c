/*
 * A069194  a(n) = Sum_{d|n} (n/d)*phi(n)/phi(d).
 * https://oeis.org/A069194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)*phi(n)/phi(d) Computation:
 *            multiplicative, a(p^e) = p^e*(p^e - p^(e-1)) +
 *            (p^(2e)-1)/(p^2-1)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation is fully static and uses trial division for factoring.
   It is suitable for n up to about 10 000 in sub‑millisecond time per call. */

#include <stdint.h>

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;                 /* a(1)=1 */
    seq_t result = 1;
    idx_t m = n;

    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {          /* find exponent e of prime p */
                m /= p;
                ++e;
            }
            /* Compute term for prime power p^e */
            seq_t pe  = pow_seq_t(p, e);           /* p^e            */
            seq_t pe1 = pe / p;                    /* p^(e-1)        */
            seq_t term1 = pe * (pe - pe1);         /* p^e(p^e-p^(e-1)) */
            seq_t num  = pow_seq_t(p, 2*e) - 1;    /* p^(2e)-1       */
            seq_t den  = (seq_t)p * (seq_t)p - 1;  /* p^2-1          */
            seq_t term2 = num / den;               /* (p^(2e)-1)/(p^2-1) */
            result *= term1 + term2;
        }
    }

    if (m > 1) {                          /* remaining prime factor */
        idx_t p = m;
        idx_t e = 1;
        seq_t pe  = p;                      /* p^1 */
        seq_t pe1 = 1;                      /* p^0 */
        seq_t term1 = pe * (pe - pe1);      /* p(p-1) */
        seq_t num  = pow_seq_t(p, 2*e) - 1;  /* p^2-1 */
        seq_t den  = (seq_t)p * (seq_t)p - 1;
        seq_t term2 = num / den;            /* =1 */
        result *= term1 + term2;
    }

    return result;
}
