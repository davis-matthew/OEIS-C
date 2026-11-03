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
 * Validated: standard, pass, 48 terms through n = 9252 (2026-11-01)
 * Helpers:   binomial, ipow, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"
#include "../utils/isqrt.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Map the sequence index n to the row i and column j of the triangle. */
static inline void map_index(idx_t n, idx_t *i, idx_t *j)
{
    /* Find i such that cumulative entries up to row i exceed n. */
    idx_t t = 8 * n + 1;               /* 8n+1 fits comfortably in idx_t */
    idx_t s = isqrt(t);     /* floor(sqrt(8n+1)) */
    idx_t t0 = (s - 1) / 2;            /* floor((sqrt(8n+1)-1)/2) */
    *i = t0;
    *j = n - (*i) * ((*i) + 1) / 2;    /* n minus preceding rows' count */
}

seq_t a(idx_t n)
{
    idx_t i, j;
    map_index(n, &i, &j);

    seq_t bin  = binomial(i, j);
    seq_t p12  = ipow((seq_t)12, (seq_t)(i - j));
    seq_t p7   = ipow((seq_t)7,  (seq_t)j);

    seq_t tmp = seq_mul(bin, p12);
    return seq_mul(tmp, p7);
}
