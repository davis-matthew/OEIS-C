/*
 * A101278  Write n in base 3 as n = b_0 + b_1*3 + b_2*3^2 + b_3*3^3 + ...;
 *          then a(n) = Product_{i >= 0} prime(i+1)^b_i.
 * https://oeis.org/A101278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = ∏_{i≥0} prime(i+1)^{b_i}, where n = Σ b_i * 3^i (base‑3
 *            digits b_i ∈ {0,1,2})
 * Range:     u8 n <= 22, u16 n <= 133, u32 n <= 1453, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    /* first 25 primes – enough for n up to 10⁶ (base‑3 length ≤ 13).  */
    static const unsigned long long primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL,
        53ULL, 59ULL, 61ULL, 67ULL, 71ULL, 73ULL, 79ULL,
        83ULL, 89ULL, 97ULL
    };

    unsigned pos = 0;
    while (n > 0) {
        unsigned digit = n % 3;   /* 0,1,2 */
        if (digit != 0) {
            seq_t p = (seq_t)primes[pos];
            if (digit == 1)
                result *= p;
            else            /* digit == 2 */
                result *= p * p;
        }
        n /= 3;
        pos++;
    }
    return result;
}
