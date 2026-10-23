/*
 * A170770  Expansion of ( phi(q) * phi(q^63) + phi(-q) * phi(-q^63) + 4 *
 *          q^16 * psi(q^2) * psi(q^126) ) / 2 in powers of q^2 where phi(),
 *          psi() are Ramanujan theta functions.
 * https://oeis.org/A170770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^(2n) in ( phi(q^7)*phi(q^9) +
 *            phi(-q^7)*phi(-q^9) + 4*q^4*psi(q^14)*psi(q^18) ) / 2 where
 *            phi(q) = Σ_{k∈ℤ} q^{k^2}, phi(-q) = Σ_{k∈ℤ} (-q)^{k^2}, psi(q) =
 *            Σ_{k≥0} q^{k(k+1)/2}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>


typedef struct { idx_t exp; __int128 coeff; } Term;

static void build_phi_terms(idx_t m, int negative, idx_t max_exp,
                            Term out[200], size_t *len)
{
    size_t cnt = 0;
    for (idx_t k = 0;; ++k) {
        idx_t e = m * k * k;
        if (e > max_exp) break;
        out[cnt].exp   = e;
        if (k == 0)
            out[cnt].coeff = 1;
        else
            out[cnt].coeff = negative ?
                              ((k & 1) ? -2 : 2) : 2;
        ++cnt;
    }
    *len = cnt;
}

static void build_psi_terms(idx_t m, idx_t max_exp,
                            Term out[200], size_t *len)
{
    size_t cnt = 0;
    for (idx_t k = 0;; ++k) {
        idx_t e = m * k * (k + 1) / 2;
        if (e > max_exp) break;
        out[cnt].exp   = e;
        out[cnt].coeff = 1;
        ++cnt;
    }
    *len = cnt;
}

seq_t a(idx_t n)
{
    idx_t target = 2 * n;          /* exponent of q we need */
    Term t7[200], t9[200], tm7[200], tm9[200];
    Term t14[200], t18[200];
    size_t l7 = 0, l9 = 0, ltm7 = 0, ltm9 = 0, lt14 = 0, lt18 = 0;

    /* Build phi terms */
    build_phi_terms(7, 0, target, t7,  &l7);
    build_phi_terms(9, 0, target, t9,  &l9);
    build_phi_terms(7, 1, target, tm7, &ltm7);
    build_phi_terms(9, 1, target, tm9, &ltm9);

    /* Build psi terms */
    build_psi_terms(14, target, t14, &lt14);
    build_psi_terms(18, target, t18, &lt18);

    /* Convolution of phi terms */
    __int128 sumA = 0, sumB = 0;
    for (size_t i = 0; i < l7; ++i)
        for (size_t j = 0; j < l9; ++j)
            if (t7[i].exp + t9[j].exp == target)
                sumA += t7[i].coeff * t9[j].coeff;

    for (size_t i = 0; i < ltm7; ++i)
        for (size_t j = 0; j < ltm9; ++j)
            if (tm7[i].exp + tm9[j].exp == target)
                sumB += tm7[i].coeff * tm9[j].coeff;

    /* Convolution of psi terms, shifted by q^4 */
    __int128 sumC = 0;
    if (target >= 4) {
        idx_t td = target - 4;
        for (size_t i = 0; i < lt14; ++i)
            for (size_t j = 0; j < lt18; ++j)
                if (t14[i].exp + t18[j].exp == td)
                    sumC += t14[i].coeff * t18[j].coeff;
        sumC *= 4;               /* multiply by 4 from the formula */
    }

    __int128 val = (sumA + sumB + sumC) / 2;
    return (seq_t)val;
}
