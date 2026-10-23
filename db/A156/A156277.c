/*
 * A156277  Dirichlet inverse of A011655, characteristic function of numbers
 *          that are not multiples of 3; Numbers appearing at every third row
 *          in the third column of A156241.
 * https://oeis.org/A156277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, mult
 * Formula:   a(n) = -μ(3n) = µ(n) for n not divisible by 3 and squarefree,
 *            else 0
 * Range:     i8 n <= 9371, i16 n <= 9371, i32 n <= 9371, i64 n <= 9371,
 *            i128 n <= 9371, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is indexed from 1 per OEIS offset */
    if (n == 0)
        return 0;               /* safety for zero index, not expected */

    /* If n is a multiple of 3, μ(3n) = 0 → a(n) = 0 */
    if (n % 3 == 0)
        return 0;

    idx_t temp = n;
    int prime_cnt = 0;          /* number of distinct prime factors of n */

    /* Trial division to factor n and detect square factors */
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            int exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
                if (exp > 1)          /* a square factor found */
                    return 0;
            }
            ++prime_cnt;
        }
    }

    /* If any prime factor remains > sqrt(original n) */
    if (temp > 1)
        ++prime_cnt;

    /* μ(n) = (-1)^prime_cnt for squarefree n (not divisible by 3) */
    return (prime_cnt & 1) ? (seq_t)(-1) : (seq_t)(1);
}
