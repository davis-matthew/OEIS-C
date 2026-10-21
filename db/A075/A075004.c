/*
 * A075004  Floor[ concatenation of n+2, n+1 and n divided by 3 ].
 * https://oeis.org/A075004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor( concat(n+2, n+1, n) / 3 )
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int digits_seq(seq_t x)
{
    int d = 1;
    while (x >= 10)
    {
        x /= 10;
        ++d;
    }
    return d;
}

static seq_t pow10_int(int exp)
{
    seq_t p = 1;
    while (exp-- > 0)
        p *= 10;
    return p;
}

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    seq_t n1 = n0 + 1;
    seq_t n2 = n0 + 2;

    int d0 = digits_seq(n0);
    int d1 = digits_seq(n1);

    seq_t p1 = pow10_int(d1 + d0);   /* 10^(digits(n1)+digits(n0))   */
    seq_t p0 = pow10_int(d0);        /* 10^(digits(n0))              */

    seq_t concat = n2 * p1 + n1 * p0 + n0;
    return concat / 3;
}
