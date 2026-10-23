/*
 * A113580  Define prime(0) = 1; then a(n) = sum prime(d), where d ranges over
 *          the decimal digits of n.
 * https://oeis.org/A113580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{d digit of n} prime(d), where prime(0)=1,
 *            prime(1)=2, prime(2)=3, prime(3)=5, prime(4)=7, prime(5)=11,
 *            prime(6)=13, prime(7)=17, prime(8)=19, prime(9)=23
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned long long primes_digit[10] = {
        1ULL, 2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL
    };
    seq_t sum = 0;
    if (n == 0)
        return (seq_t)primes_digit[0];
    while (n > 0) {
        unsigned long long d = n % 10;
        sum += (seq_t)primes_digit[d];
        n /= 10;
    }
    return sum;
}
