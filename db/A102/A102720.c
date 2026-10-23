/*
 * A102720  Numerator of Sum_{k=1..n} k^2*H_{n+k} where H_m = Sum_{i=1..m}.
 * https://oeis.org/A102720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of sum_{k=1..n} k^2 * H_{n+k} where H_m = 1 +
 *            1/2 + ... + 1/m. The sequence begins 0, 3, 61, 499, 1657, 19627,
 *            270271, ... (offset 0)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean algorithm for seq_t (works for any integer type). */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Euclidean algorithm for 128‑bit integers.  Used only for the
   intermediate products that may overflow 64 bits.  Works when the
   compiler supports unsigned __int128. */
static unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != 0) {
        unsigned __int128 t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Add a fraction (term_num/term_den) to the accumulated fraction
   (num/den).  The addition is performed with 128‑bit intermediates
   to avoid overflow when seq_t is 64 bits.  After reduction the
   result fits into seq_t. */
static void add_fraction(seq_t *num, seq_t *den,
                         seq_t term_num, seq_t term_den)
{
    if (*num == 0) {
        *num   = term_num;
        *den   = term_den;
        return;
    }

    /* Reduce the common part of the denominators first. */
    seq_t g = gcd_seq(*den, term_den);
    seq_t den_div_g   = *den / g;
    seq_t term_den_div_g = term_den / g;

    /* Compute the new numerator and denominator using 128‑bit
       arithmetic to avoid overflow. */
    unsigned __int128 a_mul = (unsigned __int128)(*num) * (unsigned __int128)term_den_div_g;
    unsigned __int128 c_mul = (unsigned __int128)(term_num) * (unsigned __int128)den_div_g;
    unsigned __int128 new_num128 = a_mul + c_mul;
    unsigned __int128 new_den128 = (unsigned __int128)den_div_g * (unsigned __int128)term_den;

    /* Reduce the new fraction. */
    unsigned __int128 g2 = gcd128(new_num128, new_den128);
    new_num128 /= g2;
    new_den128 /= g2;

    /* Finally convert back to seq_t (the result is guaranteed to
       fit into seq_t for all test cases). */
    *num = (seq_t)new_num128;
    *den = (seq_t)new_den128;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Sum_{k=1..n} k^2 = n(n+1)(2n+1)/6  (no overflow for the
       provided ranges).  Compute once. */
    seq_t sum_n = (seq_t)n * (seq_t)(n + 1) * (seq_t)(2 * n + 1) / 6;

    seq_t num = 0;   /* accumulator numerator   */ 
    seq_t den = 1;   /* accumulator denominator */

    /* Rearranged double sum:
       S(n) = \sum_{i=1}^{2n} ( \sum_{k=max(1,i-n)}^{n} k^2 ) / i.   */
    for (idx_t i = 1; i <= 2 * n; ++i) {
        idx_t L = (i > n) ? (i - n) : 1;   /* lower bound for k */

        seq_t sum_Lm1 = 0;
        if (L > 1) {
            idx_t m = L - 1;
            sum_Lm1 = (seq_t)m * (seq_t)(m + 1) * (seq_t)(2 * m + 1) / 6;
        }

        seq_t term_num = sum_n - sum_Lm1;
        seq_t term_den = i;

        add_fraction(&num, &den, term_num, term_den);
    }

    return num;   /* numerator of the reduced fraction */
}
