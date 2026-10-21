/*
 * A072590  Table T(n,k) giving number of spanning trees in complete bipartite
 *          graph K(n,k), read by antidiagonals.
 * https://oeis.org/A072590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = r^(c-1) * c^(r-1) where (r,c) are the unique positive
 *            integers such that the pair (r,c) is the n‑th element when the
 *            table T(r,c)=r^(c-1)*c^(r-1) is read by antidiagonals (r+c
 *            constant)
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 83, u64 n <= 199,
 *            u128 n <= 541, big any n
 * Helpers:   isqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
#include <stdint.h>
#include "../utils/isqrt_floor.h"

/* Power function for seq_t using exponentiation by squaring */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)           /* out of range: return 0 */
        return 0;

    /* Find diagonal number d such that d(d+1)/2 >= n and (d-1)d/2 < n */
    uint64_t t = 1 + 8ULL * (uint64_t)n;      /* 1 + 8*n fits in 64 bits */
    uint64_t s = isqrt_floor(t);              /* floor(sqrt(t)) */
    uint64_t d;                               /* diagonal index */
    if (s * s == t)
        d = (s - 1) / 2;                      /* exact root gives smallest d */
    else
        d = (s + 1) / 2;                      /* ceil( (-1 + sqrt(1+8n))/2 ) */

    /* Number of terms before diagonal d */
    uint64_t prev = (d - 1) * d / 2;

    /* Position within the diagonal (1‑based) */
    uint64_t pos = n - prev;                /* 1 <= pos <= d */

    /* Coordinates (r,c) on this diagonal:
       (1,d), (2,d-1), ..., (d,1) */
    uint64_t r = pos;                 /* row index */
    uint64_t c = d - pos + 1;         /* column index */

    /* Compute r^(c-1) * c^(r-1) */
    seq_t part1 = pow_seq((seq_t)r, (idx_t)(c - 1));
    seq_t part2 = pow_seq((seq_t)c, (idx_t)(r - 1));
    return part1 * part2;
}
