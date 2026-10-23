/*
 * A120793  Denominators of partial sums of Catalan numbers scaled by powers
 *          of -1/12.
 * https://oeis.org/A120793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of sum_{k=0}^{n} (-1)^k * Catalan(k)/12^k in
 *            lowest terms
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Euclidean algorithm for seq_t */
static seq_t my_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t num_abs  = 0;        /* absolute value of current numerator */
    seq_t den      = 1;        /* current denominator */
    seq_t pow12    = 1;        /* 12^k, current term's denominator power */
    seq_t cat      = 1;        /* Catalan(k) */
    bool sign_frac = false;    /* false = positive, true = negative */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term_abs = cat;
        bool sign_term = (k & 1) != 0;          /* (-1)^k */

        /* bring current fraction to common denominator pow12 */
        seq_t factor      = pow12 / den;
        seq_t scaled_abs  = num_abs * factor;

        /* add the new term with correct sign */
        seq_t new_abs;
        bool new_sign;
        if (sign_frac == sign_term)
        {
            new_abs = scaled_abs + term_abs;
            new_sign = sign_frac;
        }
        else
        {
            if (scaled_abs >= term_abs)
            {
                new_abs = scaled_abs - term_abs;
                new_sign = sign_frac;
            }
            else
            {
                new_abs = term_abs - scaled_abs;
                new_sign = sign_term;
            }
        }

        if (new_abs == 0)
            new_sign = false;   /* zero has no sign */

        /* reduce fraction to lowest terms */
        seq_t g = my_gcd(new_abs, pow12);
        new_abs /= g;
        den = pow12 / g;

        num_abs  = new_abs;
        sign_frac = new_sign;

        /* prepare for next iteration */
        pow12 *= 12;          /* 12^(k+1) */

        if (k < n)
        {
            idx_t next = k + 1;
            cat = cat * (seq_t)(4 * next - 2) / (seq_t)(next + 1);  /* Catalan recurrence */
        }
    }

    return den;
}
