/*
 * A055601  Number of n X n binary matrices with no zero rows.
 * https://oeis.org/A055601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n - 1)^n – the number of n×n binary matrices with no
 *            zero rows
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 by convention */

    /* 1. Compute 2^n using exponentiation by squaring */
    seq_t pow2   = (seq_t)1;               /* result */
    seq_t base   = (seq_t)2;               /* current power of two */
    idx_t exp    = n;                      /* exponent */

    while (exp)
    {
        if (exp & 1) pow2 = pow2 * base;   /* multiply when bit is set */
        base   = base * base;              /* square the base */
        exp >>= 1;
    }

    /* 2. Compute (2^n - 1)^n, again by exponentiation by squaring */
    seq_t base_dn = pow2 - (seq_t)1;       /* 2^n - 1 */
    seq_t res     = (seq_t)1;              /* result */
    base          = base_dn;
    exp           = n;

    while (exp)
    {
        if (exp & 1) res = res * base;
        base = base * base;
        exp >>= 1;
    }

    return res;
}
