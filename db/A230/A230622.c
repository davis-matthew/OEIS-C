/*
 * A230622  Values of x such that x^2 + y^2 = 17^n with x and y coprime and 0
 *          < x < y.
 * https://oeis.org/A230622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive coprime integer x with x^2 + y^2 = 17^n
 *            and 0 < x < y. It is obtained from the Gaussian integer (1 +
 *            4i)^n = a_n + b_n i, where a(n) = min(|a_n|, |b_n|)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* multiply a seq_t by 4 using only addition */
static inline seq_t mul4(seq_t v) {
    return v + v + v + v;
}

/* signed addition: computes s_out * m_out = s1*m1 + s2*m2
   s1,s2: false for positive, true for negative
   returns sign (false=positive) and magnitude (unsigned) */
static void signed_add(bool s1, seq_t m1, bool s2, seq_t m2, bool *s_out, seq_t *m_out) {
    if (s1 == s2) {
        *s_out = s1;
        *m_out = m1 + m2;
    } else {
        if (m1 >= m2) {
            *s_out = s1;
            *m_out = m1 - m2;
        } else {
            *s_out = s2;
            *m_out = m2 - m1;
        }
    }
}

/* signed subtraction: computes s_out * m_out = s1*m1 – s2*m2 */
static void signed_sub(bool s1, seq_t m1, bool s2, seq_t m2, bool *s_out, seq_t *m_out) {
    /* s1*m1 – s2*m2 = s1*m1 + (-s2)*m2 */
    signed_add(s1, m1, !s2, m2, s_out, m_out);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;

    bool a_neg = false; /* a_n sign: false = positive */
    seq_t a_mag = 1;    /* |a_1| = 1 */

    bool b_neg = false; /* b_n sign */
    seq_t b_mag = 4;    /* |b_1| = 4 */

    for (idx_t i = 2; i <= n; ++i) {
        /* compute a - 4*b */
        seq_t mul_b = mul4(b_mag);
        bool new_a_neg;
        seq_t new_a_mag;
        signed_sub(a_neg, a_mag, b_neg, mul_b, &new_a_neg, &new_a_mag);

        /* compute 4*a + b */
        seq_t mul_a = mul4(a_mag);
        bool new_b_neg;
        seq_t new_b_mag;
        signed_add(a_neg, mul_a, b_neg, b_mag, &new_b_neg, &new_b_mag);

        /* update for next iteration */
        a_neg = new_a_neg; a_mag = new_a_mag;
        b_neg = new_b_neg; b_mag = new_b_mag;
    }

    /* return the smaller magnitude */
    return (a_mag <= b_mag) ? a_mag : b_mag;
}
