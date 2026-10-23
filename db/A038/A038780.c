/*
 * A038780  An intermediate sequence for counting nonisomorphic circulant
 *          directed p^2-graphs, indexed by odd primes p.
 * https://oeis.org/A038780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   calc_phi, factor_u64, get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ((1/(p-1)) * sum_{r|p-1} φ(r) * 2^((p-1)/r))^2
 * with p = prime(n+1).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/calc_phi.h"
#include "../utils/pow2.h"

/* Computes a(n) for OEIS sequence A038780 */
seq_t a(idx_t n)
{
    /* Determine the (n+1)-th odd prime p */
    uint32_t p = get_prime(n + 1);            /* 1‑indexed */
    idx_t  pm1 = (idx_t)p - 1;                /* p‑1 */

    /* Factorize p-1 into primes and exponents */
    size_t  fact_cnt;
    uint64_t fact_pr[64], fact_exp[64];
    factor_u64((uint64_t)pm1, fact_pr, fact_exp, &fact_cnt);

    /* Enumerate all divisors of p-1 */
    seq_t divs[4096];
    size_t div_len = 1;
    divs[0] = 1; /* 1 is a divisor */

    for (size_t i = 0; i < fact_cnt; ++i) {
        uint64_t prime  = fact_pr[i];
        uint64_t exp    = fact_exp[i];
        size_t   old_len = div_len;
        seq_t    pow = 1;

        for (uint64_t k = 1; k <= exp; ++k) {
            pow *= (seq_t)prime;
            for (size_t j = 0; j < old_len; ++j) {
                divs[div_len++] = divs[j] * pow;
            }
        }
    }

    /* Compute the sum over divisors */
    seq_t sum = 0;
    seq_t pm1_seq = (seq_t)pm1;

    for (size_t i = 0; i < div_len; ++i) {
        seq_t r       = divs[i];                     /* divisor */
        seq_t phi_r   = calc_phi(r);                 /* φ(r) */
        idx_t  exp    = (idx_t)(pm1_seq / r);        /* (p-1)/r */
        seq_t term    = phi_r * pow2(exp);           /* φ(r)·2^exp */
        sum += term;
    }

    /* Divide the sum by (p-1) and square the result */
    seq_t m = sum / pm1_seq;
    return m * m;
}
