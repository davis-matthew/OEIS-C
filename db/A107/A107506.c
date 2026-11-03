/*
 * A107506  Coordination sequence for D_11 lattice.
 * https://oeis.org/A107506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{11} P_k * C(n-k) , C(t) = binom(t+10,10) P = {1,
 *            209, 7117, 73821, 317922, 643874, 643874, 317922, 73821, 7117,
 *            209, 1} The generating function is
 *            (x+1)(x^10+208x^9+…+1)/(1-x)^11, so the terms are given by the
 *            above convolution with binomial coefficients. This uses a
 *            fixed‑size re...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* binom(t+10,10) for t >= 0, t of type idx_t (uint64_t) */
static seq_t binom10(idx_t t)
{
    /* 10! = 3628800 */
    return (seq_t)(
            ((seq_t)(t + 10) * (t + 9) * (t + 8) * (t + 7) * (t + 6) *
             (t + 5) * (t + 4) * (t + 3) * (t + 2) * (t + 1)) / 3628800);
}

static const seq_t P[12] = {
    (seq_t)1ULL,
    (seq_t)209ULL,
    (seq_t)7117ULL,
    (seq_t)73821ULL,
    (seq_t)317922ULL,
    (seq_t)643874ULL,
    (seq_t)643874ULL,
    (seq_t)317922ULL,
    (seq_t)73821ULL,
    (seq_t)7117ULL,
    (seq_t)209ULL,
    (seq_t)1ULL
};

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k < 12; ++k) {
        if (n < k)
            continue;                      /* C(n-k) = 0 for n < k */
        seq_t c = binom10(n - k);
        sum += P[k] * c;
    }
    return sum;
}
