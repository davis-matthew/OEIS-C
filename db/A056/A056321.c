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
 * Helpers:   mu_of, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helpers from the library */
#include "../utils/mu_of.h"
#include "../utils/pow_seq.h"

static seq_t A056312(idx_t n)
{
    /* number of reversible strings of length n using all 5 colors */
    idx_t ceil_half = (n + 1) / 2;          /* ceil(n/2) */
    seq_t pos = 0, neg = 0;
    static const int comb[6] = {1, 5, 10, 10, 5, 1};

    for (int j = 0; j <= 5; ++j) {
        int k = 5 - j;                      /* remaining colors */
        seq_t pow_k_n     = pow_seq((seq_t)k, (int)n);
        seq_t pow_k_half  = pow_seq((seq_t)k, (int)ceil_half);
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
