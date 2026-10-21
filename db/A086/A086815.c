/*
 * A086815  a(n)=(n-1)*n^(2*n).
 * https://oeis.org/A086815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*n^(2*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* defined only for n ≥ 1, but safe */

    seq_t base = (seq_t)n;       /* n as seq_t */
    idx_t  exp  = 2 * n;         /* exponent 2*n (fits in idx_t) */

    /* fast exponentiation: base^exp */
    seq_t result = 1;
    seq_t cur    = base;
    while (exp)
    {
        if (exp & 1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }

    return ((seq_t)(n - 1)) * result;
}
