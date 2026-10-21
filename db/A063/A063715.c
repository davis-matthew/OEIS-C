/*
 * A063715  Number of solutions (r,u,s,t) in positive integers to the system
 *          of equations 1/r + 1/u = 1/n, 1/s + 1/t = 1/u.
 * https://oeis.org/A063715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n^2} tau((n + d)^2)
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau(m^2) = product over prime factors p_i^e_i of m
   of (2*e_i + 1)  */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_uint64.h"

/* Recursively generate all divisors of n^2 from its prime
   factorisation.  exp[] holds the exponents (2*e_i) of n^2. */
static void gen_divisors(const uint64_t *pr, const int *exp,
                         int idx, uint64_t cur,
                         uint64_t *dlist, size_t *dlen)
{
    if (idx == 0) {
        dlist[(*dlen)++] = cur;
        return;
    }
    for (int e = 0; e <= exp[idx - 1]; ++e) {
        gen_divisors(pr, exp, idx - 1, cur, dlist, dlen);
        cur *= pr[idx - 1];
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Factor n.  */
    uint64_t prime_n[20];
    int exp_n[20];
    int nfact;
    factor_uint64((uint64_t)n, prime_n, exp_n, &nfact);

    /* Build factorisation of n^2 (exponents doubled). */
    int exp_sq[20];
    for (int i = 0; i < nfact; ++i) exp_sq[i] = 2 * exp_n[i];

    /* Generate all divisors of n^2.  */
    static uint64_t divs[4096];
    size_t divcnt = 0;
    gen_divisors(prime_n, exp_sq, nfact, 1ULL, divs, &divcnt);

    seq_t total = 0;

    /* For each divisor, factor n + d and compute tau((n+d)^2).  */
    uint64_t prime_m[20];
    int exp_m[20];
    int mfact;

    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d = divs[i];
        uint64_t m = (uint64_t)n + d;          /* n + d */

        factor_uint64(m, prime_m, exp_m, &mfact);

        seq_t tau_sq = 1;
        for (int j = 0; j < mfact; ++j) {
            seq_t term = 2 * (seq_t)exp_m[j] + 1;
            tau_sq *= term;
        }
        total += tau_sq;
    }

    return total;
}
