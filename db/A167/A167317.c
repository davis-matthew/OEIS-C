/*
 * A167317  Totally multiplicative sequence with a(p) = 7*(p-3) for prime p.
 * https://oeis.org/A167317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(1)=1; for n>1, factor n=∏p^e, a(n)=∏(7*(p-3))^e; a(3k)=0
 * Range:     i8 n <= 7, i16 n <= 63, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n % 3 == 0) return 0;

    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            idx_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }

            seq_t base = (seq_t)7 * ((seq_t)p - (seq_t)3);
            seq_t term = 1;
            for (idx_t i = 0; i < exp; ++i)
                term *= base;

            result *= term;
        }
    }

    if (temp > 1)
    {
        seq_t base = (seq_t)7 * ((seq_t)temp - (seq_t)3);
        result *= base;
    }

    return result;
}
