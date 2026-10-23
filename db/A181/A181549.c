/*
 * A181549  a(n) = Sum_{k|n} k*mu_2(n/k).
 * https://oeis.org/A181549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} d * μ₂(n/d)
 * Range:     u8 n <= 125, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* μ₂(n) is the Möbius function of order 2:   */
/*   μ₂(p)   = 1,   μ₂(p²) = –1,   μ₂(p^e)=0 for e≥3  */
/* a is multiplicative with:                     */
/*   a(1)=1                                         */
/*   a(p)=p+1                                       */
/*   a(p^e)=p^e + p^(e-1) – p^(e-2)   for e≥2        */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* Offset 0 */
    seq_t result = 1;      /* multiplicative product  */
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* find exponent of p in temp */
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* compute contribution of this prime power */
            seq_t term;
            if (exp == 1) {
                term = (seq_t)p + 1;
            } else {
                /* compute p^exp */
                seq_t pe = 1;
                for (idx_t i = 0; i < exp; ++i) pe *= (seq_t)p;
                seq_t pe1 = pe / (seq_t)p;        /* p^(exp-1) */
                seq_t pe2 = pe1 / (seq_t)p;       /* p^(exp-2) */
                term = pe + pe1 - pe2;
            }
            result *= term;
        }
    }
    /* remaining prime factor greater than sqrt(original n) */
    if (temp > 1) {
        idx_t p = temp;
        seq_t term = (seq_t)p + 1; /* exponent is 1 */
        result *= term;
    }
    return result;
}
