/*
 * A055863  Fifth column of triangle A055858.
 * https://oeis.org/A055863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0, for 0 ≤ n ≤ 3; a(n) = n^3 * (n+1)^(n-3), for n ≥ 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    /* Convert index to the type of the sequence value */
    seq_t nseq = (seq_t) n;          /* n itself in seq_t */
    seq_t term  = nseq * nseq * nseq;/* n^3 */

    /* Compute (n+1)^(n-3) via exponentiation by squaring */
    seq_t base = nseq + (seq_t)1;     /* (n+1) in seq_t */
    idx_t  exp  = n - 3;              /* exponent is idx_t (unsigned) */
    seq_t pow  = 1;                   /* accumulator for the power */

    while (exp > 0)
    {
        if (exp & 1)
            pow *= base;
        base *= base;
        exp >>= 1;
    }

    term *= pow;                      /* final result: n^3 * (n+1)^(n-3) */
    return term;
}
