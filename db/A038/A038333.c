/*
 * A038333  Triangle whose (i,j)-th entry is binomial(i,j)*12^(i-j)*7^j.
 * https://oeis.org/A038333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*12^(i-j)*7^j, where (i,j) is obtained from
 *            n by the antidiagonal traversal of the triangle defined in the
 *            problem statement
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 38, u64 n <= 138, u128 n <= 504,
 *            big any n
 * Helpers:   _floor_isqrt_u64, big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/_floor_isqrt_u64.h"

/* Map the sequence index n to the row i and column j of the triangle. */
static inline void map_index(idx_t n, idx_t *i, idx_t *j)
{
    /* Find i such that cumulative entries up to row i exceed n. */
    idx_t t = 8 * n + 1;               /* 8n+1 fits comfortably in idx_t */
    idx_t s = _floor_isqrt_u64(t);     /* floor(sqrt(8n+1)) */
    idx_t t0 = (s - 1) / 2;            /* floor((sqrt(8n+1)-1)/2) */
    *i = t0;
    *j = n - (*i) * ((*i) + 1) / 2;    /* n minus preceding rows' count */
}

seq_t a(idx_t n)
{
    idx_t i, j;
    map_index(n, &i, &j);

    seq_t bin  = binomial(i, j);
    seq_t p12  = big_pow((seq_t)12, (seq_t)(i - j));
    seq_t p7   = big_pow((seq_t)7,  (seq_t)j);

    seq_t tmp = seq_mul(bin, p12);
    return seq_mul(tmp, p7);
}
