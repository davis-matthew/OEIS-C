/*
 * A093367  Number of n-bead necklaces using exactly three colors with no
 *          adjacent beads having the same color.
 * https://oeis.org/A093367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000031(n) - (5 + (-1)^n)/2 * A000031(n) = (1/n) *
 *            Σ_{d|n} φ(d) * 2^{n/d} * * For small seq_t (≤64 bits) the
 *            summation is performed in unsigned * __int128 to avoid overflow
 *            before the final division. For larger * seq_t values (big‑int
 *            build) the summation uses the arbitrary‑precision * ari...
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 70, u128 n <= 134,
 *            big any n
 * Helpers:   accumulate_small, big_pow, factorize_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/accumulate_small.h"
#include "../utils/factorize_n.h"

/*--------------  Recursion for big seq_t (arbitrary‑precision)  -----*/
static void
accumulate_big(idx_t idx, idx_t curr_d, seq_t curr_phi,
               const idx_t primes[], const uint8_t exps[],
               int k, idx_t orig_n, seq_t *sum)
{
    if (idx == k) {
        idx_t exp = orig_n / curr_d;
        seq_t pw = big_pow((seq_t)2, (seq_t)exp);
        *sum = *sum + (curr_phi * pw);
        return;
    }
    idx_t p = primes[idx];
    uint8_t e = exps[idx];
    idx_t powp = 1;
    seq_t phi_factor = 1; /* φ(p^0) = 1 */

    /* exponent 0 */
    accumulate_big(idx + 1, curr_d * powp, curr_phi * phi_factor,
                   primes, exps, k, orig_n, sum);

    for (uint8_t exp = 1; exp <= e; ++exp) {
        powp *= p;                                /* p^exp */
        phi_factor = (powp / p) * (p - 1);        /* φ(p^exp) */
        accumulate_big(idx + 1, curr_d * powp, curr_phi * phi_factor,
                       primes, exps, k, orig_n, sum);
    }
}

/*--------------  Main sequence function  ------------------------*/
seq_t a(idx_t n)
{
    /* The formula works for n ≥ 1; for n = 1 or 2 the result is 0. */
    if (n < 3) return 0;

    /* Factorise n once, then recurse over its divisors. */
    idx_t primes[20];
    uint8_t exps[20];
    int fact_cnt = 0;
    factorize_n(n, primes, exps, &fact_cnt);

    /* Choose computation path based on the size of seq_t. */
    if (sizeof(seq_t) <= 8) {          /* small integral type */
        __int128 sum128 = 0;
        accumulate_small(0, (idx_t)1, (seq_t)1,
                         primes, exps, fact_cnt, n, &sum128);

        seq_t result = (seq_t)(sum128 / n);
        /* subtract (5 + (-1)^n)/2 : 3 if even, 2 if odd */
        result -= (n & 1) ? (seq_t)2 : (seq_t)3;
        return result;
    } else {                            /* arbitrary‑precision type */
        seq_t sum_big = 0;
        accumulate_big(0, (idx_t)1, (seq_t)1,
                       primes, exps, fact_cnt, n, &sum_big);

        seq_t result = sum_big / (seq_t)n;
        result -= (n & 1) ? (seq_t)2 : (seq_t)3;
        return result;
    }
}
