/*
 * A056341  Number of bracelets of length n using a maximum of six different
 *          colored beads.
 * https://oeis.org/A056341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∑_{d|n} φ(d)·6^{n/d} + R(n)) / (2n) * where R(n) =
 *            n·6^{(n+1)/2} if n is odd, * = (n/2)·(6^{n/2}+6^{n/2+1}) if n is
 *            even
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   calc_phi, factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/calc_phi.h"
#include "../utils/pow_seq.h"

/* Recursive divisor generation */
static void rec_divs(const uint64_t *primes, const uint64_t *exps, size_t cnt,
                     size_t idx, uint64_t cur, idx_t n, seq_t *sum)
{
    if (idx == cnt) {
        idx_t exp = n / (idx_t)cur;
        seq_t pow6 = pow_seq((seq_t)6, (int)exp);
        seq_t term = calc_phi((seq_t)cur) * pow6;
        *sum += term;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t mul = 1;
    for (size_t e = 0; e <= exps[idx]; ++e) {
        rec_divs(primes, exps, cnt, idx + 1, cur * mul, n, sum);
        mul *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) { /* undefined for n=0, return 0 for safety */
        return 0;
    }

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t fcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &fcnt);

    /* Sum over rotations */
    seq_t sum_rot = 0;
    rec_divs(primes, exps, fcnt, 0, 1ULL, n, &sum_rot);

    /* Reflection contribution */
    seq_t sum_ref = 0;
    if (n & 1ULL) { /* odd n */
        seq_t pow_half = pow_seq((seq_t)6, (int)((n + 1) / 2));
        sum_ref = (seq_t)n * pow_half;
    } else {        /* even n */
        seq_t pow_half     = pow_seq((seq_t)6, (int)(n / 2));
        seq_t pow_half_p1  = pow_seq((seq_t)6, (int)(n / 2 + 1));
        seq_t factor       = (seq_t)(n / 2);
        sum_ref = factor * (pow_half + pow_half_p1);
    }

    /* Combine and divide by 2*n */
    seq_t total   = sum_rot + sum_ref;
    seq_t denom   = (seq_t)2 * (seq_t)n;
    seq_t result  = total / denom;

    return result;
}
