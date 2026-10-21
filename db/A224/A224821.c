/*
 * A224821  Expansion of theta_4(q)^2 * theta_4(q^3) in powers of q.
 * https://oeis.org/A224821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 147, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A224821: Expansion of theta_4(q)^2 * theta_4(q^3) in powers of q.
 *
 * Formula (by direct representation counting):
 *
 *   a(n) = Σ_{l∈ℤ} (-1)^l * Σ_{i,j∈ℤ, i²+j² = n-3l²} (-1)^{i+j}
 *
 * The inner sum counts weighted representations of the integer
 * s = n-3l² as a sum of two squares.  The outer sum runs over all
 * integer values of l such that s ≥ 0.
 */

#include <stdint.h>

static idx_t integer_sqrt(idx_t n)
{
    idx_t x = 0;
    while ((x + 1) * (x + 1) <= n) ++x;
    return x;
}

/*
 * Weighted sum of representations of s as i^2 + j^2
 * (i and j range over all integers).
 *
 * For each pair (i,j) with i²+j² = s we add the weight (-1)^{i+j}.
 */
static seq_t weighted_rep_sum(idx_t s)
{
    seq_t total = 0;
    idx_t limit_i = integer_sqrt(s);
    for (int i = -(int)limit_i; i <= (int)limit_i; ++i) {
        idx_t i2 = (idx_t)i * (idx_t)i;
        if (i2 > s) continue;  /* safety for overflow */
        idx_t rem = s - i2;
        idx_t j = integer_sqrt(rem);
        if (j * j != rem) continue;            /* no integer j */
        /* parity of (i+j) */
        unsigned parity = ((i & 1) ^ (j & 1));
        seq_t sign = (parity ? (seq_t)-1 : (seq_t)1);
        /* add contribution for j and for -j */
        total += sign;
        if (j > 0) total += sign;   /* the pair (i,-j) */
    }
    return total;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t max_l = integer_sqrt(n / 3);  /* floor sqrt of n/3 */
    for (int l = -(int)max_l; l <= (int)max_l; ++l) {
        idx_t l2 = (idx_t)l * (idx_t)l;
        if (3 * l2 > n) continue;   /* safety check */
        idx_t s = n - 3 * l2;
        seq_t f = weighted_rep_sum(s);
        seq_t sign_l = ((l & 1) == 0) ? (seq_t)1 : (seq_t)-1;
        result += sign_l * f;
    }
    return result;
}
