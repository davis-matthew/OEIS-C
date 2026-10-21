/*
 * A206917  Sum of binary palindromes in the half-open interval [2^(n-1),
 *          2^n).
 * https://oeis.org/A206917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (3/8)*2^(n+floor((n+1)/2)) Recurrence: a(n) = 8*a(n-2)
 *            for n > 3 Initial values: a(0)=0, a(1)=1, a(2)=3, a(3)=12
 *            Implementation uses fast exponentiation of 8 to avoid O(n) loops
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow8(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 8;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Handle the first four terms directly.  Negative indices or
       indices giving a negative k are avoided this way. */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 3) return 12;

    /* For n ≥ 4 the recurrence a(n) = 8*a(n-2) applies.  The
       sequence splits into two sub‑sequences (even and odd indices)
       that are powers of 8 multiplied by a constant. */
    idx_t  k;      /* number of powers of 8 needed */
    seq_t  base;   /* 3 for even n, 12 for odd n */

    if ((n & 1) == 0)        /* even index: base = 3 */
    {
        k = (n - 2) >> 1;    /* (n-2)/2 */
        base = 3;
    }
    else                    /* odd index: base = 12 */
    {
        k = (n - 3) >> 1;    /* (n-3)/2 */
        base = 12;
    }

    seq_t power = pow8(k);      /* compute 8^k efficiently */
    return base * power;        /* final value a(n) */
}
