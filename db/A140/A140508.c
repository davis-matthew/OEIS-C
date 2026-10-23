/*
 * A140508  Mobius transform applied twice to A001414.
 * https://oeis.org/A140508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^( ω(n)+1 ) * sum of distinct prime divisors of n,
 *            where ω(n) is the number of distinct primes dividing n; a(n) = 0
 *            if n has any squared prime factor
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1. Handle the trivial case n = 1 */
    if (n == 1) {
        return (seq_t)0;
    }

    /* 2. Factor n and check for square factors
       Also accumulate the sum of distinct primes and their count. */
    idx_t temp = n;
    seq_t sum = 0;            /* Sum of distinct prime factors (seq_t to avoid overflow). */
    int    k   = 0;           /* Count of distinct prime factors. */

    /* Trial division up to sqrt(n) */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                if (exp > 1) {          /* n is divisible by a squared prime */
                    return (seq_t)0;
                }
            }
            sum += (seq_t)p;
            ++k;
        }
    }

    /* Remaining prime factor (if any) */
    if (temp > 1) {
        sum += (seq_t)temp;
        ++k;
    }

    /* 3. Apply sign: + for odd k (prime or product of odd number of primes),
       - for even k (product of even number of primes). */
    if (k % 2 == 0) {
        sum = -sum;
    }

    return sum;
}
