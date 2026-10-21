/*
 * A076479  a(n) = mu(rad(n)), where mu is the Moebius-function (A008683) and
 *          rad is the radical or squarefree kernel (A007947).
 * https://oeis.org/A076479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = (-1)^ω(n), where ω(n) is the number of distinct prime
 *            divisors of n
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count distinct prime factors of n */
    idx_t temp = n;
    int cnt = 0;

    /* Handle factor 2 */
    if (temp % 2 == 0) {
        cnt++;
        while (temp % 2 == 0)
            temp /= 2;
    }

    /* Check odd factors */
    for (idx_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            cnt++;
            while (temp % i == 0)
                temp /= i;
        }
    }

    /* If residual > 1, it's a prime factor */
    if (temp > 1)
        cnt++;

    /* Return +1 for even number of primes, -1 for odd */
    return (cnt & 1) ? (seq_t)-1 : (seq_t)1;
}
