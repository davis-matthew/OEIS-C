/*
 * A225896  Number of nX4 binary arrays whose sum with another nX4 binary
 *          array containing no more than a single 1 has rows and columns in
 *          lexicographically nondecreasing order.
 * https://oeis.org/A225896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{16} Δ^k a(1) * C(n-1, k) where Δ^k a(1) are the
 *            k‑th forward differences of the first 17 terms (a(1)…a(17)), and
 *            C is the binomial coefficient. This polynomial (degree 16)
 *            reproduces all terms of sequence A225896
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 16, u64 n <= 81, u128 n <= 169,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 161 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Pre‑compute forward‑difference coefficients.
   pos[k] holds Δ^k a(1) if it is non‑negative,
   neg[k] holds -Δ^k a(1) if it is negative. */
static void init_coeff(seq_t *pos, seq_t *neg)
{
    /* first 17 terms a(1)…a(17) from the OEIS entry */
    static const seq_t base[17] = {
        11ULL,          /* a(1) */
        78ULL,          /* a(2) */
        545ULL,         /* a(3) */
        3459ULL,        /* a(4) */
        19270ULL,       /* a(5) */
        93428ULL,       /* a(6) */
        396804ULL,      /* a(7) */
        1495926ULL,     /* a(8) */
        5079770ULL,     /* a(9) */
        15751596ULL,    /* a(10) */
        45136888ULL,    /* a(11) */
        120738965ULL,   /* a(12) */
        304047638ULL,   /* a(13) */
        725911911ULL,   /* a(14) */
        1652919000ULL,  /* a(15) */
        3607621216ULL,  /* a(16) */
        7579528833ULL   /* a(17) */
    };

    /* use signed 128‑bit integers for the differences */
    static __int128 diff[17][17];
    int i, j, step, k;

    /* initial forward differences (order 0) */
    for (i = 0; i < 17; ++i)
        diff[0][i] = (__int128)base[i];

    /* higher‑order differences */
    for (step = 1; step < 17; ++step) {
        for (j = 0; j < 17 - step; ++j)
            diff[step][j] = diff[step - 1][j + 1] - diff[step - 1][j];
    }

    /* store positive and negative parts separately */
    for (k = 0; k <= 16; ++k) {
        __int128 val = diff[k][0];
        if (val >= 0)
            pos[k] = (seq_t)val, neg[k] = 0;
        else
            neg[k] = (seq_t)(-val), pos[k] = 0;
    }
}

seq_t a(idx_t n)
{
    /* coefficients computed once */
    static bool init = false;
    static seq_t pos[17], neg[17];

    if (!init) {
        init_coeff(pos, neg);
        init = true;
    }

    /* binomial coefficients C(n-1, k) for k = 0..16 */
    seq_t comb[17];
    comb[0] = 1;

    /* use idx_t for integer comparison to avoid signed overflow */
    idx_t n_minus_1 = n - 1;
    for (int k = 1; k <= 16; ++k) {
        if ((idx_t)(k - 1) > n_minus_1) {
            comb[k] = 0;
        } else {
            comb[k] = (comb[k - 1] *
                      ((seq_t)n_minus_1 - (seq_t)(k - 1))) /
                     (seq_t)k;
        }
    }

    /* sum positive and negative contributions separately */
    seq_t sum_pos = 0, sum_neg = 0;
    for (int k = 0; k <= 16; ++k) {
        if (pos[k] != 0)
            sum_pos += pos[k] * comb[k];
        if (neg[k] != 0)
            sum_neg += neg[k] * comb[k];
    }

    /* result is guaranteed non‑negative */
    return sum_pos - sum_neg;
}
