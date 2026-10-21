/*
 * A097542  a(n) = Sum_{k=1..2*n-1} J(4*n,k)*k, where J(i,j) is the Jacobi
 *          symbol.
 * https://oeis.org/A097542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..2*n-1} J(4*n,k)*k, where J(i,j) is the Jacobi
 *            symbol
 * Range:     i8 n <= 15, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute gcd for seq_t (works for integers or arbitrary precision) */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Compute Jacobi symbol (a/n).  n must be a positive odd integer.
   Return 1, -1, or 0.  Works for any integral seq_t type. */
static int jacobi_symbol(seq_t a, idx_t n)
{
    if ((n & 1) == 0)               /* even denominator → 0 */
        return 0;

    seq_t aa = a;
    seq_t nn = (seq_t)n;

    /* jacobi(0,n) = 0 */
    if (aa == 0)
        return 0;

    /* if a and n have a non‑trivial common divisor then the symbol is 0 */
    seq_t g = gcd_seq_t((aa < 0 ? -aa : aa), nn);
    if (g != 1)
        return 0;

    int res = 1;
    while (nn > 1)
    {
        /* remove powers of 2 from aa */
        int s = 0;
        while ((aa & 1) == 0)
        {
            aa >>= 1;
            ++s;
        }

        if ((s & 1) == 1)
        {
            seq_t mod8 = nn % 8;
            if (mod8 == 3 || mod8 == 5)
                res = -res;
        }

        /* swap aa and nn */
        seq_t tmp = aa;
        aa = nn;
        nn = tmp;

        if ((aa & 3) == 3 && (nn & 3) == 3)
            res = -res;

        aa %= nn;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0)
        return sum;                  /* undefined index, return 0 for safety */

    seq_t a_val = (seq_t)4 * (seq_t)n;
    idx_t limit = 2 * n;             /* upper bound exclusive */

    for (idx_t k = 1; k < limit; ++k)
    {
        if ((k & 1) == 0)            /* even k → Jacobi symbol 0 */
            continue;

        int j = jacobi_symbol(a_val, k);
        if (j != 0)
            sum += (seq_t)j * (seq_t)k;
    }
    return sum;
}
