/*
 * A230713  Values of y such that x^2 + y^2 = 37^n with x and y coprime and 0
 *          < x < y.
 * https://oeis.org/A230713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the larger of the two coprime components of (1+6i)^n,
 *            i.e. the y such that x^2 + y^2 = 37^n with 0 < x < y. The pair
 *            (x,y) arises from (1+6i)^n = r + i*s; a(n)=max(|r|,|s|)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* multiply a magnitude by a small unsigned integer (works for arbitrary‑precision seq_t) */
static inline seq_t mul_small(seq_t v, unsigned int m)
{
    return v * m;
}

/* add two signed numbers represented as magnitude + sign.
   a_mag  with sign a_neg (false = positive, true = negative),
   b_mag  with sign b_neg.
   Result is stored in *mag and *neg. */
static void add_signed(seq_t a_mag, bool a_neg,
                       seq_t b_mag, bool b_neg,
                       seq_t *mag, bool *neg)
{
    if (a_neg == b_neg) {
        *mag = a_mag + b_mag;
        *neg = a_neg;
    } else {
        if (a_mag >= b_mag) {
            *mag = a_mag - b_mag;
            *neg = a_neg;
        } else {
            *mag = b_mag - a_mag;
            *neg = b_neg;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* sequence is defined for n >= 1 */

    /* Initial values for (1+6i)^0 = 1 + 0i */
    seq_t r_mag = 1, s_mag = 0;
    bool r_neg = false, s_neg = false;

    for (idx_t k = 1; k <= n; ++k) {
        /* r_next = r - 6*s  (with signs) */
        seq_t s6mag = mul_small(s_mag, 6);
        bool s6neg = !s_neg;          /* sign of -6*s */
        seq_t r_next_mag; bool r_next_neg;
        add_signed(r_mag, r_neg, s6mag, s6neg, &r_next_mag, &r_next_neg);

        /* s_next = 6*r + s  (with signs) */
        seq_t r6mag = mul_small(r_mag, 6);
        bool r6neg = r_neg;           /* sign of 6*r */
        seq_t s_next_mag; bool s_next_neg;
        add_signed(r6mag, r6neg, s_mag, s_neg, &s_next_mag, &s_next_neg);

        /* update for next iteration */
        r_mag = r_next_mag; r_neg = r_next_neg;
        s_mag = s_next_mag; s_neg = s_next_neg;
    }

    /* Return the larger of |r| and |s|.  Sign is irrelevant for magnitude. */
    return (r_mag >= s_mag) ? r_mag : s_mag;
}
