/*
 * A189021  Apostol's second order Möbius (or Moebius) function mu_2(n).
 * https://oeis.org/A189021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = 0 if n has a prime factor with exponent >= 3; otherwise
 *            a(n) = (-1)^k where k counts the primes whose exponent equals
 *            exactly 2 in the prime factorization of n
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    idx_t temp = n;
    unsigned int parity = 0;   /* 0 if even number of exponent-2 primes,
                                 1 if odd */

    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            int exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                exp++;
                /* if a prime power exponent reaches 3, the value is 0 */
                if (exp >= 3)
                    return 0;
            }
            if (exp == 2)
                parity ^= 1;
        }
    }

    /* remaining factor (if any) is a prime with exponent 1 – no effect */
    return (parity ? -1 : 1);
}
