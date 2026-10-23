/*
 * A120997  Denominators of partial sums of Catalan numbers scaled by powers
 *          of 1/9.
 * https://oeis.org/A120997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 3^(2n - v3(S_n)), where S_n = sum_{k=0}^n C_k * 9^{n-k}
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the 3-adic valuation of a non‑negative integer x */
static idx_t v3_idx(idx_t x)
{
    idx_t c = 0;
    while (x % 3 == 0 && x > 0) {
        x /= 3;
        ++c;
    }
    return c;
}

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Exponent of 3 in the current sum N_k and its residue modulo 3 */
    idx_t v = 0;       /* v3(N_k) */
    uint8_t y = 1;     /* N_k / 3^v (mod 3), always 1 or 2 */

    /* Current Catalan numbers parameters: C_k = 3^f * r, r not divisible by 3 */
    idx_t f = 0;       /* v3(C_k) */
    uint8_t r = 1;     /* C_k / 3^f (mod 3) */

    /* Iterate k = 0 .. n-1 : add C_{k+1} to the running sum */
    for (idx_t i = 0; i < n; ++i) {
        /* k is the index of the current Catalan C_k */
        idx_t k = i;

        /* ---- Compute parameters for the next Catalan C_{k+1} ---- */
        idx_t t = 4 * k + 2;            /* numerator factor 4k+2 */
        idx_t d = k + 2;                /* denominator factor k+2 */

        /* Count powers of 3 in t and d */
        idx_t vt = 0;
        idx_t t_val = t;
        while (t_val % 3 == 0) {
            t_val /= 3;
            ++vt;
        }

        idx_t vd = 0;
        idx_t d_val = d;
        while (d_val % 3 == 0) {
            d_val /= 3;
            ++vd;
        }

        /* Next Catalan's exponent of 3 */
        idx_t f_next = f + vt - vd;

        /* Compute residue modulo 3 after removing all powers of 3 */
        uint8_t t_mod = (uint8_t)(t_val % 3);       /* 1 or 2 */
        uint8_t d_mod = (uint8_t)(d_val % 3);       /* 1 or 2 */
        uint8_t inv_d = (d_mod == 1) ? 1 : 2;       /* inverse of d_mod modulo 3 */

        uint8_t prod = (uint8_t)((r * t_mod) % 3);
        uint8_t c_next = (uint8_t)((prod * inv_d) % 3);   /* residue of C_{k+1} */

        /* ---- Update the running sum N_{k+1} = N_k*9 + C_{k+1} ---- */
        idx_t a1 = v + 2;         /* exponent from 9 * N_k            */
        idx_t a2 = f_next;        /* exponent from C_{k+1}            */
        idx_t v_new;
        uint8_t y_new;

        if (a1 < a2) {
            v_new = a1;
            y_new = y;
        } else if (a2 < a1) {
            v_new = a2;
            y_new = c_next;
        } else {                 /* a1 == a2 */
            uint8_t sum_mod = (y + c_next) % 3;
            if (sum_mod != 0) {
                v_new = a1;
                y_new = sum_mod;
            } else {
                v_new = a1 + 1;   /* cancellation of one extra factor of 3 */
                y_new = 1;        /* the new residue after division by 3 */
            }
        }

        /* Store updated values for next iteration */
        v = v_new;
        y = y_new;
        f = f_next;
        r = c_next;
    }

    /* The denominator after reduction is 3^(2*n - v) */
    idx_t exp = 2 * n - v;
    return seq_pow((seq_t)3, exp);
}
