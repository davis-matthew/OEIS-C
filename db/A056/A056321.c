/*
 * A056321  Number of primitive (aperiodic) reversible strings with n beads
 *          using exactly five different colors.
 * https://oeis.org/A056321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * A056312(n/d) where A056312(m) =
 *            Sum_{j=0..5} (-1)^j * C(5,j) * ( ( (5-j)^m + (5-j)^{ceil(m/2)} )
 *            / 2 )
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helpers from the library */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

static seq_t A056312(idx_t n)
{
    /* number of reversible strings of length n using all 5 colors */
    idx_t ceil_half = (n + 1) / 2;          /* ceil(n/2) */
    seq_t pos = 0, neg = 0;
    static const int comb[6] = {1, 5, 10, 10, 5, 1};

    for (int j = 0; j <= 5; ++j) {
        int k = 5 - j;                      /* remaining colors */
        seq_t pow_k_n     = ipow_i32((seq_t)k, (int)n);
        seq_t pow_k_half  = ipow_i32((seq_t)k, (int)ceil_half);
        seq_t part        = (pow_k_n + pow_k_half) / 2;
        seq_t term        = part * (seq_t)comb[j];

        if ((j & 1) == 0)   /* even j -> positive sign */
            pos += term;
        else                /* odd j -> negative sign */
            neg += term;
    }
    return pos - neg;   /* pos >= neg for all n */
}

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            int mu = mu_of(d);
            if (mu != 0) {
                idx_t m = n / d;
                seq_t val = A056312(m);
                if (mu == 1)
                    pos += val;
                else
                    neg += val;
            }
            /* paired divisor n/d if distinct */
            idx_t d2 = n / d;
            if (d2 != d) {
                mu = mu_of(d2);
                if (mu != 0) {
                    idx_t m = n / d2;   /* equals d */
                    seq_t val = A056312(m);
                    if (mu == 1)
                        pos += val;
                    else
                        neg += val;
                }
            }
        }
    }
    return pos - neg;   /* guaranteed non‑negative */
}
