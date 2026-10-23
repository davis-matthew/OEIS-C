/*
 * A104721  Expansion of (1+x)^2/(1-4*x^2).
 * https://oeis.org/A104721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (9*2^n + (-2)^n - 2*0^n)/8 (offset 0)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 the special term a(0) = 1. */
    if (n == 0)
        return (seq_t)1;

    /* Compute 2^n using binary exponentiation.  */
    seq_t pow2 = (seq_t)1;
    seq_t base  = (seq_t)2;
    idx_t e = n;
    while (e != 0) {
        if (e & 1)          /* if current bit is set, multiply current power */
            pow2 *= base;
        base *= base;       /* square the base (2^(2^k)) */
        e >>= 1;            /* move to the next bit */
    }

    /* For odd n, (-2)^n = -2^n, so the formula collapses to a(n) = 2^n. */
    if (n & 1ULL)   /* odd */
        return pow2;

    /* For even n, (-2)^n = +2^n, hence a(n) = 5*2^n/4.  */
    /* 2^n is divisible by 4 for n >= 2, so we can divide first to keep
       the intermediate product small. */
    seq_t tmp = pow2 / (seq_t)4;
    return tmp * (seq_t)5;
}
