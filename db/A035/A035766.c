/*
 * A035766  Coordination sequence for C_29 lattice.
 * https://oeis.org/A035766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2*n)] ((1+x)/(1-x))^29
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 12, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(a,b) for non‑negative a,b */
static seq_t binom_idx(idx_t a, idx_t b)
{
    if (b > a) return (seq_t)0;
    if (b == 0 || b == a) return (seq_t)1;
    if (b > a - b) b = a - b;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= b; ++i) {
        res = res * (a - b + i) / i;
    }
    return res;
}

/* Constant binomial coefficients C(29,i) */
static seq_t binom_29(idx_t i)
{
    /* Compute with the same function to keep code short */
    return binom_idx(29, i);
}

seq_t a(idx_t n)
{
    /* We need the coefficient of x^(2n) in ((1+x)/(1-x))^29. */
    idx_t k = n + n;            /* k = 2*n */
    seq_t sum = (seq_t)0;

    for (idx_t i = 0; i <= 29 && i <= k; ++i) {
        seq_t c1 = binom_29(i);                                   /* C(29,i) */
        seq_t c2 = binom_idx(28 + k - i, k - i);                 /* C(28+k-i, k-i) */
        sum += c1 * c2;
    }

    return sum;
}
