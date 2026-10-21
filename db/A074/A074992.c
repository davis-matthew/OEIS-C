/*
 * A074992  a(n) = (10^(2*n) + 10^n + 1)/3.
 * https://oeis.org/A074992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^(2*n) + 10^n + 1)/3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow10(idx_t e)
{
    seq_t result = 1;
    seq_t base   = 10;
    idx_t exp = e;
    while (exp)
    {
        if (exp & 1U)
            result = result * base;
        base = base * base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p10   = pow10(n);          /* 10^n  */
    seq_t p100  = p10 * p10;         /* 10^(2n) */
    seq_t num   = p100 + p10 + (seq_t)1;
    return num / (seq_t)3;
}
