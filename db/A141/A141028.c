/*
 * A141028  A triangular sequence of the expansion of: (1-Prime[n])^n:
 *          t(n,m)=(-1)^m*Prime[n]^(n - m)*Binomial[n, m]; with Prime[0]=1
 *          defined to extend and lower the results.
 * https://oeis.org/A141028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabl, sign
 * Formula:   a(n) = (-1)^m * Prime[r]^(r-m) * C(r, m) where r is the unique
 *            non‑negative integer such that r*(r+1)/2 < n <= (r+1)*(r+2)/2,
 *            and m = n - r*(r+1)/2 - 1
 * Range:     i8 n <= 10, i16 n <= 15, i32 n <= 36, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   big_pow, binomial, get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    /* Find the row index r */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 < n)
        ++r;

    /* Position within the row (0‑based) */
    idx_t prev   = r * (r + 1) / 2;
    idx_t offset = n - prev;          /* 1 .. r+1 */
    idx_t m      = offset - 1;        /* 0 .. r */

    /* Retrieve the prime for this row; Prime[0] = 1 by definition */
    seq_t prime = (r == 0) ? (seq_t)1 : get_prime(r);

    /* Compute prime^(r-m) */
    seq_t exp  = (seq_t)(r - m);
    seq_t powp = big_pow(prime, exp);

    /* Binomial coefficient C(r, m) */
    seq_t bin = binomial(r, m);

    /* Product */
    seq_t result = seq_mul(bin, powp);

    /* Apply the sign (-1)^m */
    if (m & 1)
        result = -result;

    return result;
}
