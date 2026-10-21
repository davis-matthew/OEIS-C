/*
 * A099590  2^(n-1) times coefficient of x in (1+x)^n mod U(n,x), U the
 *          Chebyshev polynomials.
 * https://oeis.org/A099590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A099590:
   a(n) = n * 2^(n-1)                      if n is even
   a(n) = n * 2^(n-1) - (n+1)/2 * (-1)^((n-1)/2)   if n is odd
   (Indexing starts at n = 1) */
seq_t a(idx_t n)
{
    /* 2^(n-1) – computed by repeated left shifts (safe for all seq_t sizes) */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i + 1 < n; ++i)
        pow2 <<= 1;            /* multiplies by 2 each iteration */

    seq_t n_seq = (seq_t)n;
    seq_t base  = n_seq * pow2; /* n * 2^(n-1) */

    if ((n & 1) == 0)          /* even n => a(n) = base */
    {
        return base;
    }
    else                       /* odd n => adjust by (n+1)/2 * (-1)^((n-1)/2) */
    {
        idx_t t = (n + 1) / 2;      /* (n+1)/2 */
        idx_t k = (n - 1) / 2;      /* (n-1)/2 */

        if ((k & 1) == 0)          /* (-1)^k = +1 : subtract t */
        {
            return base - (seq_t)t;
        }
        else                       /* (-1)^k = -1 : add t */
        {
            return base + (seq_t)t;
        }
    }
}
