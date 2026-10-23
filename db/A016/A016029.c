/*
 * A016029  a(1) = a(2) = 1, a(2n + 1) = 2*a(2n) and a(2n) = 2*a(2n - 1) +
 *          (-1)^n.
 * https://oeis.org/A016029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive definition:
   a(1) = a(2) = 1
   a(2n)   = 2*a(2n-1) + (-1)^n
   a(2n+1) = 2*a(2n)
   This implementation iterates from 1 up to n, using only a single
   variable of type seq_t. */
seq_t a(idx_t n)
{
    /* handle the offset (n >= 1 is assumed by the caller) */
    seq_t val = 1;           /* a(1) */
    if (n == 1)
        return val;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(i) = 2 * a(i-1) */
        val = val * 2;

        /* even index: add or subtract 1 depending on n/2 parity */
        if ((i & 1U) == 0)
        {
            idx_t half = i >> 1;          /* n = i/2  */
            if ((half & 1U) == 0)
                val += 1;                  /* (-1)^n = +1 when n even */
            else
                val -= 1;                  /* (-1)^n = -1 when n odd  */
        }
        /* odd indices already correct after doubling */
    }
    return val;
}
