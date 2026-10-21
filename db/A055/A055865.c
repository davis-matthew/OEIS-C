/*
 * A055865  Second column of triangle A055864.
 * https://oeis.org/A055865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)^(n-2) (a(1) = 0)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A055865. */

seq_t a(idx_t n)
{
    if (n <= 1u)
        return 0;                     /* a(1) = 0, a(0) undefined */

    seq_t base = (seq_t)(n + 1);        /* (n+1) as seq_t            */
    idx_t  exp  = n - 2;                /* exponent (n-2)            */
    seq_t  result = 1;                  /* pow = (n+1)^(n-2)         */

    while (exp)
    {
        if (exp & 1u)
            result *= base;              /* result *= base if bit set */
        base *= base;                    /* base squared              */
        exp >>= 1;                       /* shift exponent            */
    }

    return (seq_t)n * result;           /* a(n) = n * result         */
}
