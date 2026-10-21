/*
 * A051797  Partial sums of A007585.
 * https://oeis.org/A051797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3,3) * (2*n+1) = (n+1)*(n+2)*(n+3)*(2*n+1)/6
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 335, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* local copies in seq_t to allow large integer arithmetic */
    seq_t a = (seq_t)n + 1;            /* (n+1) */
    seq_t b = (seq_t)n + 2;            /* (n+2) */
    seq_t c = (seq_t)n + 3;            /* (n+3) */
    seq_t d = ((seq_t)n * 2) + 1;      /* (2*n+1) */

    /* divide by 2 using one of the consecutive numbers */
    if ((a & 1) == 0)       a /= 2;
    else if ((b & 1) == 0)  b /= 2;
    else                    c /= 2;   /* one of them is even */

    /* divide by 3 using one of the consecutive numbers */
    if (a % 3 == 0)          a /= 3;
    else if (b % 3 == 0)     b /= 3;
    else                     c /= 3;   /* one of them is divisible by 3 */

    /* now the product fits in seq_t even for the 64‑bit build up to n≈10⁴ */
    seq_t res = a;
    res *= b;
    res *= c;
    res *= d;
    return res;
}
