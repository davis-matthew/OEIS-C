/*
 * A164095  a(n) = 2*a(n-2) for n > 2; a(1) = 5, a(2) = 6.
 * https://oeis.org/A164095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 60, u64 n <= 124,
 *            u128 n <= 252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 2*a(n-2) with a(1)=5, a(2)=6 */

static seq_t pow_two(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;
    idx_t e = exp;

    while (e > 0)
    {
        if (e & 1)
            result = result * base;
        base = base * base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 5;
    if (n == 2)
        return 6;

    if (n & 1)                 /* n is odd: a(n) = 5 * 2^((n-1)/2) */
    {
        idx_t exp = n / 2;     /* (n-1)/2 */
        return (seq_t)5 * pow_two(exp);
    }
    else                      /* n is even: a(n) = 6 * 2^(n/2 - 1) */
    {
        idx_t exp = n / 2 - 1;
        return (seq_t)6 * pow_two(exp);
    }
}
