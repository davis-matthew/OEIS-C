/*
 * A094302  a(1) = 1; if n > a(n-1) then a(n) = n*a(n-1) else a(n) = least
 *          integer multiple of a(n-1)/n.
 * https://oeis.org/A094302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 278, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   For n > 1:
     if n > a(n-1)      a(n) = n * a(n-1);
     else               a(n) = a(n-1) / gcd(a(n-1), n)   (gcd returns positive integer) */

static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    /* Euclidean algorithm – works for any unsigned integer type */
    while (b != (seq_t)0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= (idx_t)1)
        return (seq_t)1;                 /* a(1) == 1, a(0) is unused */

    seq_t val = (seq_t)1;                /* a(1) */
    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        seq_t ii = (seq_t)i;
        if (ii > val)                    /* n > a(n-1) */
            val = val * ii;              /* multiply */
        else
        {
            seq_t g = gcd_seq_t(val, ii);/* gcd of a(n-1) and n */
            val = val / g;               /* divide by gcd */
        }
    }
    return val;
}
