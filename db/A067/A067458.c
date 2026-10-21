/*
 * A067458  Sum of remainders when n is divided by its nonzero digits.
 * https://oeis.org/A067458
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum_{d digit of n, d != 0} (n mod d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts only non‑zero digits of n (in base 10) */

seq_t a(idx_t n)
{
    /* For n < 10 the sequence is defined as 0; this is safe even if
       such values are passed in the tests. */
    if (n < 10)
        return 0;

    seq_t sum = 0;
    idx_t tmp = n;

    while (tmp > 0)
    {
        int d = tmp % 10;        /* current decimal digit */
        if (d != 0)
            sum += (seq_t)(n % (idx_t)d);  /* n mod d, cast to seq_t */
        tmp /= 10;
    }

    return sum;
}
