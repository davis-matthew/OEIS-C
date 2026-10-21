/*
 * A094151  Remainder when concatenation 1,2,3,...up to (n-1) is divided by n.
 * https://oeis.org/A094151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007908(n-1) mod n (concatenation of 1..n‑1 modulo n)
 * Range:     u8 n <= 279, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                   /* a(1) = 0 */

    seq_t mod = (seq_t)n;           /* n fits in seq_t for all n <= 10000 */
    seq_t cur = 0;

    /* powers of 10 up to 10^5, enough for digits of i < 10000 */
    static const seq_t pow10[6] = {1, 10, 100, 1000, 10000, 100000};

    for (idx_t i = 1; i < n; ++i)
    {
        /* number of decimal digits of i */
        idx_t d;
        if (i < 10)            d = 1;
        else if (i < 100)      d = 2;
        else if (i < 1000)     d = 3;
        else if (i < 10000)    d = 4;
        else                   d = 5;   /* for completeness if n > 10000 */

        cur = (cur * pow10[d] + (seq_t)i) % mod;
    }

    return cur;
}
