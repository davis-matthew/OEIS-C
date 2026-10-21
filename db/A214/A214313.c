/*
 * A214313  a(n) is the number of all five-color bracelets (necklaces with
 *          turning over allowed) with n beads and the four colors are from a
 *          repertoire of n distinct colors, for n >= 5.
 * https://oeis.org/A214313
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,5) * A056345(n) (n ≥ 5) where A056345(n) =
 *            number of distinct bracelets of length n on an alphabet of size
 *            5 that use all 5 colors at least once. This is computed by
 *            Burnside's Lemma with exact surjective color assignments to the
 *            cycle blocks of each symmetry
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   _binom_n5, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/_binom_n5.h"

/* Euler's totient of an unsigned 64‑bit integer */
static seq_t _phi_u64(idx_t m)
{
    seq_t res = m;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            res -= res / p;
        }
    }
    if (m > 1) res -= res / m;
    return res;
}

/* Surjective colorings from a set of size c onto 5 colors
   (S(c,5)*5! = 5^c - 5*4^c + 10*3^c - 10*2^c + 5,  c ≥ 5)          */
static seq_t _term_for_cycles(idx_t c)
{
    if (c < 5) return (seq_t)0;
    seq_t pow5 = big_pow((seq_t)5, (seq_t)c);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)c);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)c);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)c);
    seq_t term = pow5;
    term -= pow4 * 5;
    term += pow3 * 10;
    term -= pow2 * 10;
    term += 5;
    return term;
}

/* A056345(n): bracelets of length n on 5 colors using all 5 colors */
static seq_t _A056345(idx_t n)
{
    /* rotational contributions */
    seq_t rot_sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* d is a divisor, add contribution for d and n/d */
            seq_t term_d = _term_for_cycles(d);
            rot_sum += term_d * _phi_u64(n / d);
            if (d != n / d) {
                seq_t term_nd = _term_for_cycles(n / d);
                rot_sum += term_nd * _phi_u64(d);
            }
        }
    }

    /* reflective contributions */
    seq_t ref_sum = 0;
    if (n & 1) { /* odd n : one fixed point + (n-1)/2 two‑cycles */
        idx_t c = (n + 1) >> 1;
        seq_t term = _term_for_cycles(c);
        ref_sum = term * n;          /* n reflections of this type */
    } else { /* even n : two types of reflections */
        idx_t c1 = n / 2 + 1;          /* two fixed points + pairs */
        idx_t c2 = n / 2;              /* no fixed points, only pairs */
        seq_t term1 = _term_for_cycles(c1);
        seq_t term2 = _term_for_cycles(c2);
        ref_sum = (term1 + term2) * (n / 2);   /* n/2 reflections of each */
    }

    seq_t total = rot_sum + ref_sum;
    seq_t denom = (seq_t)(2 * n);
    return total / denom;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;           /* undefined for n<5 */
    seq_t comb = _binom_n5(n);
    seq_t so = _A056345(n);
    return comb * so;
}
