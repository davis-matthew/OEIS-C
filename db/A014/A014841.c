/*
 * A014841  Sum modulo the base of all the digits of n in every base from 2 to
 *          n-1.
 * https://oeis.org/A014841
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{b=2}^{n-1} (digit_sum(n,b) mod b)
 * Range:     u8 n <= 37, u16 n <= 607, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where digit_sum(n,b) = sum of the digits of n written in base b */

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is undefined; return 0 to avoid errors. */
    if (n < 3) return 0;

    seq_t total = 0;

    for (idx_t b = 2; b < n; ++b)
    {
        idx_t m  = n;
        seq_t sum = 0;

        /* digit sum of n in base b */
        while (m > 0)
        {
            sum += (seq_t)(m % b);
            m /= b;
        }

        /* (digit sum) mod b, added to the running total */
        total += sum % (seq_t)b;
    }

    return total;
}
