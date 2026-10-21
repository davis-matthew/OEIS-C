/*
 * A208534  Number of n-bead necklaces of 7 colors not allowing reversal, with
 *          no adjacent beads having the same color.
 * https://oeis.org/A208534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7 (n=1); a(n) = (1/n) * Σ_{d|n} φ(d) * ( 6^{n/d} +
 *            (-1)^{n/d} * 6 ) for n ≥ 2 (k = 7 colors, adjacency restriction,
 *            rotations only)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

static void factor(idx_t n, idx_t *primes, idx_t *exps, int *cnt)
{
    int c = 0;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            primes[c]   = p;
            exps[c]     = e;
            ++c;
        }
    }
    if (n > 1) {
        primes[c] = n;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

static void gen_divs(const idx_t *primes, const idx_t *exps, int pf_cnt,
                     int idx, idx_t cur, idx_t *divs, size_t *cnt)
{
    if (idx == pf_cnt) {
        divs[(*cnt)++] = cur;
        return;
    }
    idx_t p   = primes[idx];
    idx_t mul = cur;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divs(primes, exps, pf_cnt, idx + 1, mul, divs, cnt);
        mul *= p;
    }
}

static idx_t phi_of(idx_t n)
{
    if (n == 0) return 0;
    idx_t phi = n;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            phi -= phi / p;
        }
    }
    if (temp > 1) phi -= phi / temp;
    return phi;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* Undefined for n=0, safe guard. */
    if (n == 1) return (seq_t)7;

    /* Factor n and generate all divisors. */
    idx_t primes[16], exps[16];
    int pf_cnt = 0;
    factor(n, primes, exps, &pf_cnt);

    idx_t divs[512];
    size_t div_cnt = 0;
    gen_divs(primes, exps, pf_cnt, 0, 1, divs, &div_cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        idx_t d = divs[i];
        idx_t phi = phi_of(d);
        idx_t m   = n / d;

        seq_t pow6 = seq_pow((seq_t)6, m);
        seq_t F = pow6;
        if ((m & 1) == 0) {   /* even m */
            F += (seq_t)6;
        } else {               /* odd m */
            F -= (seq_t)6;
        }

        seq_t prod = seq_mul(F, (seq_t)phi);
        sum += prod;
    }

    return sum / (seq_t)n;
}
