/*
 * A229110  Sum of non-divisors of n reduced modulo n.
 * https://oeis.org/A229110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A024816(n) mod n A024816(n) = sum_{k=1..n} k - σ(n) σ(n)
 *            = sum of all divisors of n Hence a(n) = ( n*(n+1)/2 - σ(n) ) mod
 *            n
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9439 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute σ(n) modulo n                                       */
static seq_t sigma_mod_u64(idx_t n)
{
    seq_t mod = (seq_t)n;
    seq_t prod = 1;                 /* product of factors modulo n */
    idx_t tmp = n;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                ++exp;
                tmp /= p;
            }
            /* term = 1 + p + p^2 + … + p^exp  (mod n) */
            seq_t term  = 1;
            seq_t pow   = 1;
            for (int i = 0; i < exp; ++i) {
                pow  = (pow * (seq_t)p) % mod;
                term = (term + pow) % mod;
            }
            prod = (prod * term) % mod;
        }
    }

    if (tmp > 1) {                 /* remaining prime factor */
        seq_t term = ((seq_t)1 + (seq_t)tmp) % mod;
        prod = (prod * term) % mod;
    }

    return prod;
}

/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/* main sequence function                                       */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* safety guard – not used for OEIS */

    seq_t mod        = (seq_t)n;
    seq_t sigma_mod  = sigma_mod_u64(n);

    /* n_half = n/2 if n even, otherwise 0 */
    seq_t n_half = ((n & 1) == 0) ? (seq_t)(n / 2) : (seq_t)0;

    seq_t result;
    if (n_half >= sigma_mod)
        result = n_half - sigma_mod;
    else
        result = (seq_t)n + n_half - sigma_mod;   /* (n_half - sigma_mod) mod n */

    return result;
}
