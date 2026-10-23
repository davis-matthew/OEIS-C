/*
 * A101943  Write n in base 5 as n = b_0 + b_1*5 + b_2*5^2 + b_3*5^3 + ...;
 *          then a(n) = Product_{i >= 0} prime(i+1)^b_i.
 * https://oeis.org/A101943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = Π_{i≥0} prime(i+1)^{b_i}, where b_i are the base‑5 digits
 *            of n
 * Range:     u8 n <= 18, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* number of base‑5 digits of n */
    idx_t temp = n;
    idx_t digits = 0;
    while (temp > 0) {
        temp /= 5;
        digits++;
    }

    /* generate enough primes on first use */
    static idx_t primes[200];
    static idx_t prime_cnt = 0;
    if (prime_cnt < digits) {
        idx_t candidate = prime_cnt ? primes[prime_cnt - 1] + 1 : 2;
        while (prime_cnt < digits) {
            int is_prime = 1;
            for (idx_t i = 0; i < prime_cnt; ++i) {
                idx_t p = primes[i];
                if (candidate % p == 0) {
                    is_prime = 0;
                    break;
                }
                if (p * p > candidate) break;
            }
            if (is_prime) {
                primes[prime_cnt++] = candidate;
            }
            ++candidate;
        }
    }

    /* compute the product from the base‑5 digits */
    seq_t result = 1;
    temp = n;
    idx_t pi = 0;
    while (temp > 0) {
        idx_t digit = temp % 5;
        temp /= 5;
        if (digit > 0) {
            seq_t p = (seq_t)primes[pi];
            seq_t pow = 1;
            for (idx_t k = 0; k < digit; ++k) pow *= p;
            result *= pow;
        }
        ++pi;
    }
    return result;
}
