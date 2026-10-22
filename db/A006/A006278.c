/*
 * A006278  a(n) is the product of the first n primes congruent to 1 (mod 4).
 * https://oeis.org/A006278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} A002144(i), where A002144 are primes ≡ 1
 *            (mod 4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"


seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t candidate = 5;
    idx_t count = 0;
    
    while (count < n) {
        if (is_prime(candidate)) {
            result *= (seq_t)candidate;
            count++;
        }
        candidate += 4;
    }
    
    return result;
}
