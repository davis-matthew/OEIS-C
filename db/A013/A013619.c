/*
 * A013619  Triangle of coefficients in expansion of (1+12*x)^n.
 * https://oeis.org/A013619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = 12^k * binomial(m, k) where m = floor((sqrt(8*n + 1) - 1)
 *            / 2), k = n - m*(m+1)/2. ░ The sequence is obtained by reading
 *            the triangle T(m, k) = 12^k * binomial(m, k) in row‑major order
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   floor_sqrt, pow12_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/pow12_u.h"

/* Compute binomial coefficient C(a, b) for a, b <= 2^32. */
static seq_t binom_u(idx_t a, idx_t b)
{
    if (b > a) return 0;
    if (b > a - b) b = a - b;    /* symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i)
    {
        /* res = res * (a - b + i) / i        */
        res = res * (a - b + i) / i;   /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t n_seq  = (seq_t)n;
    seq_t tmp    = n_seq * 8 + 1;          /* 8*n + 1  */
    seq_t sqt    = floor_sqrt(tmp);        /* floor(sqrt(8*n+1)) */
    idx_t m      = (idx_t)((sqt - 1) / 2);  /* m = floor((sqrt(8n+1)-1)/2) */
    seq_t t_seq  = (seq_t)m * (m + 1) / 2;  /* triangular number of m */
    idx_t t      = (idx_t)t_seq;
    idx_t k      = n - t;                  /* k = n - m*(m+1)/2 */
    seq_t bin    = binom_u(m, k);
    seq_t pwr    = pow12_u(k);
    return bin * pwr;
}
