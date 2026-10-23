/*
 * A045973  a(1)=10; if n = Product p_i^e_i, n > 1, then a(n) = Product
 *          p_{i+1}^e_i * Product p_{i+3}^e_i.
 * https://oeis.org/A045973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=10; if n = ∏ p_i^e_i, n>1, then a(n)=∏ (p_{i+1}^e_i *
 *            p_{i+3}^e_i)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   isPrimeGeneric, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* Find the next prime greater than x */
static inline uint64_t next_prime_uint64(uint64_t x)
{
    do {
        ++x;
    } while (!isPrimeGeneric((seq_t)x));
    return x;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)10;

    uint64_t num = (uint64_t)n;
    seq_t result = (seq_t)1;

    for (uint64_t p = 2; p * p <= num; ++p) {
        if (num % p == 0) {
            int exp = 0;
            while (num % p == 0) {
                num /= p;
                ++exp;
            }
            uint64_t q1 = next_prime_uint64(p);
            uint64_t q3 = next_prime_uint64(next_prime_uint64(next_prime_uint64(p)));

            seq_t term1 = seq_pow((seq_t)q1, (idx_t)exp);
            seq_t term3 = seq_pow((seq_t)q3, (idx_t)exp);
            result = seq_mul(result, term1);
            result = seq_mul(result, term3);
        }
    }

    /* Remaining prime factor > sqrt(original n) */
    if (num > 1) {
        uint64_t p = num;
        int exp = 1;
        uint64_t q1 = next_prime_uint64(p);
        uint64_t q3 = next_prime_uint64(next_prime_uint64(next_prime_uint64(p)));

        seq_t term1 = seq_pow((seq_t)q1, (idx_t)exp);
        seq_t term3 = seq_pow((seq_t)q3, (idx_t)exp);
        result = seq_mul(result, term1);
        result = seq_mul(result, term3);
    }

    return result;
}
